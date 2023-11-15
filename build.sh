#!/usr/bin/env bash
set -e
cc `pkg-config sdl2 --libs --cflags` -Wall -pedantic -std=gnu11 main.c -o snake
echo "build complete, running..."
./snake
