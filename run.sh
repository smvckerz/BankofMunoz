#!/usr/bin/env bash
# BankCore Environment Bootstrapper
# Cross-platform: macOS, Linux, and Windows (via Git Bash / MSYS2 / Cygwin / WSL)

set -u  # treat unset variables as errors; intentionally not using -e so we can handle failures inline

echo "========================================="
echo "   BankCore Environment Bootstrapper     "
echo "========================================="

# ---------------------------------------------------------------------------
# 1. Detect the operating system
# ---------------------------------------------------------------------------
OS_RAW="$(uname -s 2>/dev/null || echo Unknown)"

case "$OS_RAW" in
    Darwin*)                    PLATFORM="macos"   ;;
    Linux*)                     PLATFORM="linux"   ;;
    MINGW*|MSYS*|CYGWIN*)       PLATFORM="windows" ;;  # Git Bash, MSYS2, Cygwin
    *)                          PLATFORM="unknown" ;;
esac

# WSL reports as Linux, but distinguishing it can be useful for path handling.
if [ "$PLATFORM" = "linux" ] && grep -qiE "(microsoft|wsl)" /proc/version 2>/dev/null; then
    PLATFORM="wsl"
fi

echo "[*] Detected platform: $PLATFORM ($OS_RAW)"

# ---------------------------------------------------------------------------
# 2. Helper: install MySQL on each platform
# ---------------------------------------------------------------------------
install_mysql_macos() {
    if ! command -v brew >/dev/null 2>&1; then
        echo "[!] Homebrew is missing. Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" \
            || { echo "[x] Homebrew install failed."; exit 1; }
    fi

    if ! command -v mysql >/dev/null 2>&1; then
        echo "[*] Installing MySQL via Homebrew..."
        brew install mysql || { echo "[x] MySQL install failed."; exit 1; }
    fi

    if ! brew services list | grep -q "mysql.*started"; then
        echo "[*] Starting MySQL background service..."
        brew services start mysql
        sleep 3
    fi
}

install_mysql_linux() {
    if command -v mysql >/dev/null 2>&1; then
        return 0
    fi

    if command -v apt-get >/dev/null 2>&1; then
        echo "[*] Installing MySQL via apt..."
        sudo apt-get update && sudo apt-get install -y mysql-server
    elif command -v dnf >/dev/null 2>&1; then
        echo "[*] Installing MySQL via dnf..."
        sudo dnf install -y mysql-server
    elif command -v yum >/dev/null 2>&1; then
        echo "[*] Installing MySQL via yum..."
        sudo yum install -y mysql-server
    elif command -v pacman >/dev/null 2>&1; then
        echo "[*] Installing MariaDB (MySQL-compatible) via pacman..."
        sudo pacman -Sy --noconfirm mariadb
    elif command -v zypper >/dev/null 2>&1; then
        echo "[*] Installing MySQL via zypper..."
        sudo zypper install -y mysql
    else
        echo "[x] No supported package manager found (apt/dnf/yum/pacman/zypper)."
        exit 1
    fi

    # Start the service — try systemd first, then fall back to service(8).
    if command -v systemctl >/dev/null 2>&1; then
        sudo systemctl enable --now mysql 2>/dev/null \
            || sudo systemctl enable --now mysqld 2>/dev/null \
            || sudo systemctl enable --now mariadb 2>/dev/null
    elif command -v service >/dev/null 2>&1; then
        sudo service mysql start 2>/dev/null \
            || sudo service mysqld start 2>/dev/null \
            || sudo service mariadb start 2>/dev/null
    fi
}

install_mysql_windows() {
    if command -v mysql >/dev/null 2>&1; then
        return 0
    fi

    # winget ships with Windows 10/11; choco is a popular community alternative.
    if command -v winget >/dev/null 2>&1; then
        echo "[*] Installing MySQL via winget..."
        winget install --id Oracle.MySQL --silent --accept-package-agreements --accept-source-agreements
    elif command -v choco >/dev/null 2>&1; then
        echo "[*] Installing MySQL via Chocolatey..."
        choco install mysql -y
    else
        echo "[!] Neither winget nor Chocolatey is available."
        echo "    Install MySQL manually from https://dev.mysql.com/downloads/installer/"
        echo "    or install winget/choco and re-run this script."
        exit 1
    fi

    # Try to start the service. The exact name varies by installer version.
    if command -v sc >/dev/null 2>&1; then
        sc start MySQL80 >/dev/null 2>&1 || sc start MySQL >/dev/null 2>&1 || true
    fi
}

case "$PLATFORM" in
    macos)              install_mysql_macos   ;;
    linux|wsl)          install_mysql_linux   ;;
    windows)            install_mysql_windows ;;
    *)
        echo "[x] Unsupported or unidentified platform: $OS_RAW"
        echo "    Run this script under macOS, Linux, WSL, Git Bash, MSYS2, or Cygwin."
        exit 1
        ;;
esac

# ---------------------------------------------------------------------------
# 3. Provision the target database if missing
# ---------------------------------------------------------------------------
echo "[*] Checking database schema setup..."

# Try the common auth paths in order: socket auth, empty password, then prompt.
if mysql -u root -e "CREATE DATABASE IF NOT EXISTS bankcore;" >/dev/null 2>&1; then
    echo "[+] Schema ready (socket / passwordless auth)."
elif mysql -u root --password="" -e "CREATE DATABASE IF NOT EXISTS bankcore;" >/dev/null 2>&1; then
    echo "[+] Schema ready (empty password)."
else
    echo "[!] Could not auto-provision the schema. MySQL likely needs a password."
    echo "    Set MYSQL_PWD in your environment or run:"
    echo "    mysql -u root -p -e \"CREATE DATABASE IF NOT EXISTS bankcore;\""
    # Don't hard-exit — let the user decide whether to continue.
fi

# ---------------------------------------------------------------------------
# 4. Build the application
# ---------------------------------------------------------------------------
echo "[*] Building application source..."
if ! command -v make >/dev/null 2>&1; then
    echo "[x] 'make' is not installed. Install build tools and re-run."
    case "$PLATFORM" in
        macos)      echo "    Try: xcode-select --install" ;;
        linux|wsl)  echo "    Try: sudo apt-get install build-essential   (or your distro's equivalent)" ;;
        windows)    echo "    Try: winget install GnuWin32.Make           (or use MSYS2's make)" ;;
    esac
    exit 1
fi

if ! make; then
    echo "[x] Build failed. Aborting before launch."
    exit 1
fi

# ---------------------------------------------------------------------------
# 5. Hand off to the application binary
# ---------------------------------------------------------------------------
APP_BIN="./db"
if [ "$PLATFORM" = "windows" ] && [ -x "./db.exe" ]; then
    APP_BIN="./db.exe"
fi

if [ ! -x "$APP_BIN" ]; then
    echo "[x] Application binary '$APP_BIN' not found or not executable."
    exit 1
fi

echo "[+] Environment ready. Launching BankCore application..."
echo "========================================="
exec "$APP_BIN"