cmd_/home/pi/segment/segment.mod := printf '%s\n'   segment.o | awk '!x[$$0]++ { print("/home/pi/segment/"$$0) }' > /home/pi/segment/segment.mod
