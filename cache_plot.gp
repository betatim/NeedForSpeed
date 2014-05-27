#!/bin/sh
gnuplot << EOF
reset
set terminal png

plot "$1"
EOF
