#!/bin/bash
g++ lhid.cpp -lXtst -lX11 -I include/ -o build/leapHID ./lib/libLeap.so
