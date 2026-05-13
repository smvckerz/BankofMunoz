#ifndef CONFIG_H
#define CONFIG_H

// BankCore — compile-time default connection parameters.
//
// These act as fallbacks. At runtime, matching environment variables
// (DB_HOST, DB_PORT, DB_USER, DB_PASSWORD, DB_NAME) override these.
//
// Copy this file to `config.h` and tailor it to your local setup.
// `config.h` is gitignored — never commit real credentials.

#define DB_HOST     "localhost"
#define DB_PORT     3306
#define DB_USER     "root"
#define DB_PASSWORD "YOUR_PASSWORD_HERE"
#define DB_NAME     "bankcore"

#endif