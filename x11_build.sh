#!/usr/bin/env sh

mkdir -p ./build

gcc $(pkg-config x11 --cflags --libs) test.c -o build/test
