# syntax=docker/dockerfile:1.7
#
# BankCore — multi-stage build
# Stage 1 compiles the C++ binary against libmysqlclient.
# Stage 2 ships only the binary + runtime libs as a minimal, non-root image.
#
# Credentials are NOT baked into the image. They are read from environment
# variables at runtime (DB_HOST, DB_PORT, DB_USER, DB_PASSWORD, DB_NAME),
# falling back to whatever was compiled into include/config.h.

# ---------------------------------------------------------------------------
# Stage 1: Builder
# ---------------------------------------------------------------------------
FROM debian:bookworm-slim AS builder

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        default-libmysqlclient-dev \
        pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

COPY src/      ./src/
COPY include/  ./include/
COPY Makefile  ./

# Use placeholder defaults at build time — real values come from env at runtime.
RUN cp include/config.h.example include/config.h \
    && make

# ---------------------------------------------------------------------------
# Stage 2: Runtime
# ---------------------------------------------------------------------------
FROM debian:bookworm-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        libmariadb3 \
    && rm -rf /var/lib/apt/lists/* \
    && useradd --create-home --shell /bin/bash --uid 1000 bankcore

WORKDIR /app

COPY --from=builder --chown=bankcore:bankcore /build/db ./db

USER bankcore

CMD ["./db"]
