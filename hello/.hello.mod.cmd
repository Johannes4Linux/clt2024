cmd_/home/pi/hello/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/pi/hello/"$$0) }' > /home/pi/hello/hello.mod
