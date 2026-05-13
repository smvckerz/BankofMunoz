#!/bin/bash

echo "========================================="
echo "   BankCore Environment Bootstrapper    "
echo "========================================="

# 1. Detect Operating System
OS="$(uname -s)"

if [ "$OS" = "Darwin" ]; then
    echo "[*] Detecting macOS Environment..."
    
    # Check for Homebrew
    if ! command -v brew &> /dev/null; then
        echo "[!] Homebrew is missing. Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL githubusercontent.com)"
    fi

    # Check for MySQL
    if ! command -v mysql &> /dev/null; then
        echo "[*] Installing MySQL via Homebrew..."
        brew install mysql
    fi

    # Ensure MySQL Service is running
    if ! brew services list | grep -q "mysql.*started"; then
        echo "[*] Starting MySQL background service..."
        brew services start mysql
        sleep 3 # Give daemon a moment to open sockets
    fi

elif [ "$OS" = "Linux" ]; then
    echo "[*] Detecting Linux Environment..."
    
    if ! command -v mysql &> /dev/null; then
        echo "[*] Installing MySQL Server..."
        sudo apt-get update && sudo apt-get install -y mysql-server
    fi
    
    sudo systemctl start mysql

else
    echo "[!] Windows environment detected. Please install MySQL manually via installer."
    exit 1
fi

# 2. Automatically provision the target database if missing
echo "[*] Checking database schema setup..."
mysql -u root -e "CREATE DATABASE IF NOT EXISTS bankcore;" 2>/dev/null

if [ $? -ne 0 ]; then
    echo "[!] Warning: Could not auto-provision schema without password."
    echo "[*] Attempting fallback with empty password..."
    mysql -u root --password="" -e "CREATE DATABASE IF NOT EXISTS bankcore;"
fi

# 3. Compile fresh updates using your project Makefile
echo "[*] Building application source..."
make

# 4. Handoff system thread directly to your app execution layer
echo "[+] Environment secure. Launching BankCore application..."
echo "========================================="
./db