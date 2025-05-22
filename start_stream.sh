#!/bin/bash

# Settings perfect for Pi 3 B+
WIDTH=800
HEIGHT=600
FPS=12
PORT=8080

# Start the camera stream
libcamera-vid -t 0 \
  --width $WIDTH --height $HEIGHT \
  --framerate $FPS \
  --inline \
  --codec h264 \
  -o - | \
ffmpeg -i - \
  -c:v copy \
  -f mpegts \
  udp://0.0.0.0:$PORT?pkt_size=1316