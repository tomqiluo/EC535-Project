#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define CCS811_ADDR 0x5B
#define SI7021_ADDR 0x40

void ccs811_init(int file)
{
    char config[2] = {0};
    config[0] = 0x01;
    config[1] = 0x10;
    write(file, config, 2);
}

float si7021_read_humidity(int file)
{
    char data[2] = {0};
    char cmd = 0xF5;
    write(file, &cmd, 1);
    usleep(20000);
    read(file, data, 2);
    return ((125.0 * ((data[0] << 8) | data[1])) / 65536.0) - 6.0;
}

float si7021_read_temperature(int file)
{
    char data[2] = {0};
    char cmd = 0xF3;
    write(file, &cmd, 1);
    usleep(20000);
    read(file, data, 2);
    return ((175.72 * ((data[0] << 8) | data[1])) / 65536.0) - 46.85;
}

int main()
{
    int file1, file2;
    char filename[20];
    int adapter_nr = 2; /* Change to 1 for Beaglebone Black Wireless */
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    file1 = open(filename, O_RDWR);
    if (file1 < 0) {
        perror("Failed to open the i2c bus for CCS811");
        exit(1);
    }
    file2 = open(filename, O_RDWR);
    if (file2 < 0) {
        perror("Failed to open the i2c bus for SI7021");
        exit(1);
    }
    if (ioctl(file1, I2C_SLAVE, CCS811_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to CCS811 slave");
        exit(1);
    }
    if (ioctl(file2, I2C_SLAVE, SI7021_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to SI7021 slave");
        exit(1);
    }

    ccs811_init(file1);

    while (1) {
        char data[8] = {0};
        data[0] = 0x02;
        if (write(file1, data, 1) != 1) {
            perror("Failed to write to the i2c bus");
        }
        usleep(10000);
        if (read(file1, data, 8) != 8) {
            perror("Failed to read from the i2c bus");
        }
        int co2 = (data[0] << 8) | data[1];
        int voc = (data[2] << 8) | data[3];
        float humidity = si7021_read_humidity(file2);
        float temperature = si7021_read_temperature(file2);
        printf("CO2: %d ppm, VOC: %d ppb, Humidity: %.2f %%RH, Temperature: %.2f C\n", co2, voc, humidity, temperature);
        usleep(500000);
    }
    return 0;
}
