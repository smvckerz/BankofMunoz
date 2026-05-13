# BankCore

A terminal-based banking system built in C++ with MySQL. Manage bank accounts through an interactive, menu-driven CLI with direct MySQL C API integration.

---

## Features

- Create new bank accounts with a starting balance
- View all existing accounts
- Persistent storage via MySQL
- SQL injection protection via `mysql_real_escape_string`

---

## Requirements

- C++ compiler (`g++` recommended)
- MySQL server running locally
- MySQL C API / `libmysqlclient`

---

## Setup

### 1. Clone the repository

```bash
https://github.com/smvckerz/BankofMunoz.git (https)
git@github.com:smvckerz/BankofMunoz.git (SSH)

cd bankcore
```

### 2. Configure database credentials

```bash
cp include/config.h.example include/config.h
```

Open `include/config.h` and fill in your MySQL password and database name.

### 3. Set up the MySQL database

Log into MySQL and run the following:

```sql
CREATE DATABASE bankcore;

USE bankcore;

CREATE TABLE accounts (
    id      INT AUTO_INCREMENT PRIMARY KEY,
    name    VARCHAR(255)   NOT NULL,
    balance DECIMAL(10, 2) NOT NULL
);
```

### 4. Build the project

```bash
make
```

### 5. Run

```bash
./run.sh
```

Or run the compiled binary directly.

---

## Project Structure

```
├── src/
│   ├── main.cpp        # Entry point
│   ├── database.cpp    # MySQL connection and query logic
│   ├── menu.cpp        # Interactive CLI menu
│   └── banner.cpp      # ASCII banner
├── include/
│   ├── database.h
│   ├── menu.h
│   ├── banner.h
│   ├── config.h        # Your local credentials (gitignored)
│   └── config.h.example
├── makefile
└── run.sh
```
