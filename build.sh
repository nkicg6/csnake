#!/usr/bin/env bash
set -ex
clang `pkg-config sdl2 --libs --cflags` -Wall -g -O0 -pedantic -std=gnu11 main.c -o snake
./snake
