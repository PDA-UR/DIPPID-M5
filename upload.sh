#!/bin/bash

# Passes network credentials to platformIO,
# which are then set as constants in the program.
# This way, the source code does not have to be changed
# to connect to a network or host.

# usage:
# sh upload.sh SSID PW IP PORT

SSID=$1
PW=$2
IP=$3
PORT=$4

FLAGS=""

FLAGS="$FLAGS -DSSID=\"$SSID\""
FLAGS="$FLAGS -DPW=\"$PW\""
FLAGS="$FLAGS -DIP=\"$IP\""
FLAGS="$FLAGS -DPORT=\"$PORT\""

export PLATFORMIO_BUILD_FLAGS="$FLAGS"

pio run --target=upload
