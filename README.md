<h1 align="center">💳 BankCore</h1>

<p align="center">
  <em>A terminal-native banking system in C++ with a MySQL backbone.</em>
</p>

<p align="center">
  <a href="https://github.com/smvckerz/BankofMunoz/actions/workflows/ci.yml">
    <img alt="CI"          src="https://github.com/smvckerz/BankofMunoz/actions/workflows/ci.yml/badge.svg">
  </a>
  <img alt="C++17"         src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white">
  <img alt="MySQL 8"       src="https://img.shields.io/badge/MySQL-8.0-4479A1?logo=mysql&logoColor=white">
  <img alt="Docker"        src="https://img.shields.io/badge/Docker-ready-2496ED?logo=docker&logoColor=white">
  <img alt="License: MIT"  src="https://img.shields.io/badge/License-MIT-yellow.svg">
</p>

---

## ✨ What it is

BankCore is a self-contained banking CLI: account creation, balance management, persistent storage — all driven through a menu-driven terminal interface with an animated rainbow startup banner. It talks to MySQL directly via the C API (no ORM), with proper input escaping to keep SQL injection at bay.

```
    ____              __            ____   __  ___
   / __ )____ _____  / /__   ____  / __/  /  |/  /_  ______  ____  ____
  / __  / __ `/ __ \/ //_/  / __ \/ /_   / /|_/ / / / / __ \/ __ \/_  /
 / /_/ / /_/ / / / / ,<    / /_/ / __/  / /  / / /_/ / / / / /_/ / / /_
/_____/\__,_/_/ /_/_/|_|   \____/_/    /_/  /_/\__,_/_/ /_/\____/ /___/
```

---

## 🚀 Quick start

The only prerequisite is **[Docker Desktop](https://www.docker.com/products/docker-desktop/)**.

```bash
git clone git@github.com:smvckerz/BankofMunoz.git
cd BankofMunoz
./bankcore
```

That's it. The launcher script bootstraps `.env` from the template, brings up a MySQL 8 container, builds the app image, and drops you straight into the interactive menu.

> **Windows:** run via Git Bash, MSYS2, or WSL — the launcher is a POSIX shell script.

---

## 🎛️ Commands

| Command              | What it does                                       |
| -------------------- | -------------------------------------------------- |
| `./bankcore`         | Start everything and launch the interactive menu   |
| `./bankcore stop`    | Shut down containers, **preserve** account data    |
| `./bankcore reset`   | Shut down and **wipe** the data volume (prompts)   |
| `./bankcore logs`    | Tail the MySQL container logs                      |
| `./bankcore help`    | Print the available subcommands                    |

---

## 🧱 Architecture

```
┌─────────────────────────────────┐         ┌────────────────────────┐
│        bankcore-app             │         │      bankcore-db       │
│   (Debian slim, non-root)       │  ──→    │   (mysql:8.0 official) │
│   • C++17 binary                │  TCP    │   • schema/init.sql    │
│   • libmariadb3 runtime         │ :3306   │   • named volume       │
│   • reads env at runtime        │         │   • healthchecked      │
└─────────────────────────────────┘         └────────────────────────┘
                    ↑                                  ↑
                    └──── docker-compose.yml ──────────┘
                         (bankcore-net bridge)
```

**Connection config** is resolved at runtime in this order:

1. Environment variables — `DB_HOST`, `DB_PORT`, `DB_USER`, `DB_PASSWORD`, `DB_NAME`
2. Compile-time defaults baked into `include/config.h`

For Docker, env vars come from `.env` via `docker-compose.yml`. For native builds, you set them yourself (or rely on the `config.h` defaults).

---

## 🛠️ Tech stack

- **Language:** C++17
- **Database driver:** MySQL C API / `libmysqlclient` (MariaDB connector on Debian)
- **Build:** GNU Make with portable `mysql_config` detection
- **Container runtime:** Multi-stage Docker build → minimal `debian:bookworm-slim` runtime
- **CI:** GitHub Actions (native build on Ubuntu + macOS, Docker image build)

---

## 🧪 Manual build (no Docker)

For when you want to build directly against a host MySQL.

<details>
<summary><strong>Click to expand</strong></summary>

### Prerequisites

| Platform        | Packages                                                                       |
| --------------- | ------------------------------------------------------------------------------ |
| macOS           | Xcode CLI tools, Homebrew, `mysql-client`, `pkg-config`                        |
| Debian / Ubuntu | `build-essential`, `default-libmysqlclient-dev`, `mysql-server`, `pkg-config`  |
| Fedora / RHEL   | `gcc-c++`, `make`, `mysql-devel`, `mysql-server`, `pkgconf-pkg-config`         |
| Arch            | `base-devel`, `mariadb`, `mariadb-libs`, `pkgconf`                             |

### Steps

```bash
# 1. Set compile-time defaults (overridable via env vars at runtime)
cp include/config.h.example include/config.h
$EDITOR include/config.h

# 2. Provision the database
mysql -u root -p < schema/init.sql

# 3. Build
make

# 4. Run
./db
```

</details>

---

## 📁 Project structure

```
bankcore/
├── src/                  # C++ implementation
│   ├── main.cpp
│   ├── database.cpp
│   ├── menu.cpp
│   └── banner.cpp
├── include/              # Headers + config template
│   ├── *.h
│   └── config.h.example
├── schema/
│   └── init.sql          # Auto-loaded by the mysql container
├── .github/workflows/
│   └── ci.yml            # Native + Docker build verification
├── bankcore              # Launcher script (POSIX shell)
├── Dockerfile            # Multi-stage build → non-root runtime
├── docker-compose.yml    # app + db orchestration
├── Makefile              # Portable, mysql_config-aware
├── .env.example          # Template for credentials
└── README.md
```

---

## 📜 License

[MIT](LICENSE) — © Eduardo Munoz
