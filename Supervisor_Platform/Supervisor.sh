#!/bin/bash

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

HOSTIP="192.168.0.108"

echo "Supervisor Init Starting..........."
chmod 777 /dev/input/js0
chmod 777 /dev/ttyUSB0
sleep 2s
echo "you may now run the Supervisor Platform"
#echo "attempting to stream from $HOSTIP on Port $PORT"
