-- BankCore — initial schema
-- Idempotent: safe to run repeatedly.

CREATE DATABASE IF NOT EXISTS bankcore
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE bankcore;

CREATE TABLE IF NOT EXISTS accounts (
    id      INT AUTO_INCREMENT PRIMARY KEY,
    name    VARCHAR(255)   NOT NULL,
    balance DECIMAL(15, 2) NOT NULL DEFAULT 0.00,
    created_at TIMESTAMP   NOT NULL DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_name (name)
) ENGINE=InnoDB;