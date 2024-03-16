#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

int main(int argc, char **argv) {
	int fd;
	struct gpiohandle_request led;
	struct gpiohandle_data data;

	if(argc == 1) {
		printf("Usage: %s DEVFILE VALUE\n", argv[0]);
		return 1;
	}

	/* Open the gpiochip device file */
	fd = open(argv[1], O_RDWR);
	if(fd < 0) {
		perror("Error opening gpiochip0");
		return -1;
	}

	/* Setup LED to output */
	led.flags = GPIOHANDLE_REQUEST_OUTPUT;
	strcpy(led.consumer_label, "LED");
	memset(led.default_values, 0, sizeof(led.default_values));
	led.lines = 1;
	led.lineoffsets[0] = 25;

	if(ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &led) < 0) {
		perror("Error setting GPIO 25 to output");
		close(fd);
		return -1;
	}

	/* Let's set the LED */
	if(argc >= 3)
		data.values[0] = atoi(argv[2]);
	else
		data.values[0] = 0;
	if(ioctl(led.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0) 
		perror("Error setting GPIO to 1");

	close(fd);
	close(led.fd);
	return 0;
}

