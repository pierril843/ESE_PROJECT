#!/bin/bash

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

HOSTIP="$(hostname -I)"
PORT="8070"


echo "Embedded Init Starting..........."
chmod 777 /dev/ttyUSB0
chmod 777 /dev/video0
sleep 2s
echo "you may now run the Embedded Platform"
echo "attempting to stream over $HOSTIP on Port $PORT"
mjpg_streamer -i "input_uvc.so -d /dev/video0 -r VGA -y -f 50 " -o "output_http.so -p $PORT -w /home/mate/mjpg-streamer-code-182/mjpg-streamer/www"
