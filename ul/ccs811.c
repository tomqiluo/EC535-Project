#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define CCS811_ADDR 0x5B

void ccs811_init(int file)
{
    char config[2] = {0};
    config[0] = 0x01;
    config[1] = 0x10;
    write(file, config, 2);
}

int main()
{
    int file;
    char filename[20];
    int adapter_nr = 2; /* Change to 1 for Beaglebone Black Wireless */
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    file = open(filename, O_RDWR);
    if (file < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }
    if (ioctl(file, I2C_SLAVE, CCS811_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        exit(1);
    }

    ccs811_init(file);

    while (1) {
        char data[8] = {0};
        data[0] = 0x02;
        write(file, data, 1);
        usleep(10000);
        read(file, data, 8);
        int co2 = (data[0] << 8) | data[1];
        int voc = (data[2] << 8) | data[3];
        printf("CO2: %d ppm, VOC: %d ppb\n", co2, voc);
        usleep(500000);
    }
    return 0;
}
