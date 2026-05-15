# BankCore — Makefile
#
# Builds the `db` binary. Finds `mysql_config` portably across:
#   - Linux (in PATH after installing libmysqlclient-dev / mariadb-devel)
#   - Apple Silicon Homebrew (/opt/homebrew/opt/mysql-client/bin)
#   - Intel Homebrew         (/usr/local/opt/mysql-client/bin)
#   - Docker Debian          (PATH after default-libmysqlclient-dev)

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -Iinclude

# 1. PATH lookup
MYSQL_CONFIG := $(shell command -v mysql_config 2>/dev/null)

# 2. Common Homebrew fallbacks (Apple Silicon + Intel)
ifeq ($(MYSQL_CONFIG),)
  MYSQL_CONFIG := $(firstword $(wildcard \
    /opt/homebrew/opt/mysql-client/bin/mysql_config \
    /opt/homebrew/opt/mysql/bin/mysql_config \
    /usr/local/opt/mysql-client/bin/mysql_config \
    /usr/local/opt/mysql/bin/mysql_config))
endif

ifeq ($(MYSQL_CONFIG),)
  $(error mysql_config not found. Install MySQL client dev libraries: \
    apt: default-libmysqlclient-dev | brew: mysql-client | dnf: mysql-devel)
endif

MYSQL_FLAGS := $(shell $(MYSQL_CONFIG) --cflags --libs)

SRC := src/main.cpp src/banner.cpp src/database.cpp src/menu.cpp src/actions.cpp
HDR := include/banner.h include/database.h include/menu.h include/config.h
BIN := db

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC) $(MYSQL_FLAGS)

clean:
	rm -f $(BIN) $(BIN).exe