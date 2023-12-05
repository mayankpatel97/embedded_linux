#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>


int main()
{
    // Create I2C bus
    uint8_t data[2];	    
    int file;
    char *bus = "/dev/i2c-1";
    if((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the bus. \n");
        exit(1);
    }
    // Get I2C device, PCF8574 I2C address is 0x20(32)
    if(ioctl(file, I2C_SLAVE, 0x20) < 0)
    {
	perror("Error setting slave address");
	close(file);
    }
    
    data[0] = 0x0;
    data[1] = 0xfe;
    if(write(file, data,2 ) != 2) {
	perror("Error writing to i2c bus");
	close(file);
	}
    /* set the led state */
    data[0] = 0x14;
    data[1] = 0x00;
    if(write(file, data,2) != 2) {
	perror("Error writing i2c data");
	close(file);
	
	}
    return 0;
}
