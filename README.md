# BankCore

A terminal-based banking system written in C++ with direct MySQL C API integration. Manage bank accounts through an interactive, menu-driven CLI.

## Features

- Create new bank accounts with a starting balance
- View all existing accounts
- Persistent storage via MySQL
- SQL injection protection via `mysql_real_escape_string`
- Animated rainbow ASCII banner on startup (Windows + Unix ANSI support)

## Tech stack

- **Language:** C++17
- **Database:** MySQL 8.0 (via `libmysqlclient` / MariaDB connector)
- **Build:** GNU Make with portable `mysql_config` detection
- **Runtime:** Docker (recommended) or native Linux / macOS

---

## Quick start

The only prerequisite is [Docker Desktop](https://www.docker.com/products/docker-desktop/). Once it's installed and running:

```bash
git clone git@github.com:smvckerz/BankofMunoz.git
cd BankofMunoz
./bankcore
```

That's it. The script handles everything: creates your `.env` from the template on first run, brings up MySQL, builds the app container, and drops you into the interactive menu.

### Other commands

```bash
./bankcore         # start the app (default)
./bankcore stop    # shut down containers, preserve data
./bankcore reset   # shut down AND wipe all account data
./bankcore logs    # tail the MySQL container logs
./bankcore help    # show usage
```

> **Windows users:** Run via Git Bash, MSYS2, or WSL — the launcher is a POSIX shell script. PowerShell / CMD aren't supported directly.

---

## Configuration

BankCore reads connection parameters in this order (highest precedence first):

1. **Environment variables** — `DB_HOST`, `DB_PORT`, `DB_USER`, `DB_PASSWORD`, `DB_NAME`
2. **Compile-time defaults** — values from `include/config.h`

This means the same binary works in local dev, Docker, and production without rebuilding. For Docker, credentials come from `.env`. For native builds, edit `include/config.h`.

---

## Manual setup (native build, no Docker)

If you'd rather build and run on the host directly.

### Prerequisites

| Platform        | Required packages                                            |
| --------------- | ------------------------------------------------------------ |
| macOS           | Xcode CLI tools, Homebrew, `mysql-client`, `pkg-config`      |
| Debian / Ubuntu | `build-essential`, `default-libmysqlclient-dev`, `mysql-server`, `pkg-config` |
| Fedora / RHEL   | `gcc-c++`, `make`, `mysql-devel`, `mysql-server`, `pkgconf-pkg-config` |
| Arch            | `base-devel`, `mariadb`, `mariadb-libs`, `pkgconf`           |

### Steps

```bash
# 1. Configure defaults (used if env vars aren't set)
cp include/config.h.example include/config.h
$EDITOR include/config.h

# 2. Provision the database
mysql -u root -p < schema/init.sql

# 3. Build
make

# 4. Run
./db
```

---

## Project structure

```
bankcore/
├── src/                # implementation
│   ├── main.cpp        # entry point
│   ├── database.cpp    # MySQL connection + query logic
│   ├── menu.cpp        # interactive CLI menu
│   └── banner.cpp      # animated ASCII banner
├── include/            # headers
│   ├── database.h
│   ├── menu.h
│   ├── banner.h
│   ├── config.h        # local credentials (gitignored)
│   └── config.h.example
├── schema/
│   └── init.sql        # database + table DDL
├── .github/workflows/
│   └── ci.yml          # GitHub Actions build verification
├── bankcore            # launcher script
├── Dockerfile          # multi-stage build, non-root runtime
├── docker-compose.yml  # app + mysql orchestration
├── Makefile            # portable build rules
├── .env.example        # template for docker-compose
└── README.md
```

## License

See [LICENSE](LICENSE).
