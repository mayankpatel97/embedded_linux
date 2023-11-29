#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

int main()
{
	int fd;
	struct gpiohandle_request led, button;
	struct gpiohandle_data data;
	
	/* open the gpio chip file */
	fd = open("/dev/gpiochip0",O_RDWR);
	if(fd < 0) {
		perror("Error");
		return -1;
	}

	/* setup led output */
	led.flags = GPIOHANDLE_REQUEST_OUTPUT;
	strcpy(led.consumer_label, "LED");
	memset(led.default_values, 0, sizeof(led.default_values));
	led.lines = 1;
	led.lineoffsets[0] = 16; // pin num
	if(ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &led) <0) {
		perror("ERROR setting gpio 16 to input.");
		close(fd);
		return -1;
	}

	/* setup led button to input */
        button.flags = GPIOHANDLE_REQUEST_INPUT;
        strcpy(button.consumer_label, "LED");
        memset(button.default_values, 0, sizeof(button.default_values));
        button.lines = 1;
        button.lineoffsets[0] = 2; // pin num
        if(ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &button) < 0) {
                perror("ERROR setting gpio 2 to input.");
                close(led.fd);
		close(fd);
                return -1;
        }

	data.values[0] = 1;
	if(ioctl(led.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0)
		perror("Error setting GPIO to 1");

	if(ioctl(button.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) <0)
		perror("Error setting GPIO to 1");

	printf("Button is %s\n", data.values[0] >0 ? "pressed" : "not pressed");
	sleep(2);
	close(fd);


	return 0;
}
