#!/bin/bash

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
echo $PLATFORMIO_BUILD_FLAGS

pio run --target=upload
