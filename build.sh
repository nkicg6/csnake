#!/usr/bin/env bash
set -ex
cc `pkg-config sdl2 --libs --cflags` -Wall -pedantic -std=gnu11 main.c -o snake
./snake
