#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>


#define CCS811_ADDR 0x5B
#define SI7021_ADDR 0x40

#define MQ7_PIN "/sys/class/gpio/gpio67/value"
#define BUFFER_SIZE 100

#define GPIO_PATH "/sys/class/gpio"

void ccs811_init(int file)
{
    char config[2] = {0};
    config[0] = 0x01;
    config[1] = 0x10;
    if (write(file, config, 2) != 2) {
        perror("Failed to write to CCS811 sensor");
    }
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
    int gpio68_fd, gpio44_fd, gpio26_fd;
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

    int mq7_pin_fd;
    char buffer[BUFFER_SIZE];
    int value;
    float voltage;
    float co_ppm;

    // Export GPIO pin for MQ-7 sensor
    system("echo 67 > /sys/class/gpio/export");
    // Set direction of GPIO pin to input
    system("echo in > /sys/class/gpio/gpio67/direction");

    // Set the direction of the GPIO pins to output
    gpio68_fd = open(GPIO_PATH "/gpio68/direction", O_WRONLY);
    write(gpio68_fd, "out", strlen("out"));
    close(gpio68_fd);

    gpio44_fd = open(GPIO_PATH "/gpio44/direction", O_WRONLY);
    write(gpio44_fd, "out", strlen("out"));
    close(gpio44_fd);

    gpio26_fd = open(GPIO_PATH "/gpio26/direction", O_WRONLY);
    write(gpio26_fd, "out", strlen("out"));
    close(gpio26_fd);

    // Turn on the LED lights
    gpio68_fd = open(GPIO_PATH "/gpio68/value", O_WRONLY);
    write(gpio68_fd, "1", strlen("1"));
    close(gpio68_fd);

    gpio44_fd = open(GPIO_PATH "/gpio44/value", O_WRONLY);
    write(gpio44_fd, "1", strlen("1"));
    close(gpio44_fd);

    gpio26_fd = open(GPIO_PATH "/gpio26/value", O_WRONLY);
    write(gpio26_fd, "1", strlen("1"));
    close(gpio26_fd);

    // Wait for a second
    sleep(1);

    // Turn off the LED lights
    gpio68_fd = open(GPIO_PATH "/gpio68/value", O_WRONLY);
    write(gpio68_fd, "0", strlen("0"));
    close(gpio68_fd);

    gpio44_fd = open(GPIO_PATH "/gpio44/value", O_WRONLY);
    write(gpio44_fd, "0", strlen("0"));
    close(gpio44_fd);

    gpio26_fd = open(GPIO_PATH "/gpio26/value", O_WRONLY);
    write(gpio26_fd, "0", strlen("0"));
    close(gpio26_fd);

    while (1) {
        FILE *fp;
        char str[100];

        fp = fopen("data.txt", "a");
        if(fp == NULL) {
           perror("Error opening file");
           return(-1);
        }

        char data[8] = {0};
        data[0] = 0x02;
        if (write(file1, data, 1) != 1) {
            perror("Failed to write to the i2c bus");
        }
        usleep(50000);
        if (read(file1, data, 8) != 8) {
            perror("Failed to read from the i2c bus");
        }
        int co2 = (data[0] << 8) | data[1];
        int voc = (data[2] << 8) | data[3];
        float humidity = si7021_read_humidity(file2);
        float temperature = si7021_read_temperature(file2);

        // Open GPIO pin for reading
        mq7_pin_fd = open(MQ7_PIN, O_RDONLY);
        if (mq7_pin_fd == -1) {
            printf("Error opening MQ-7 pin file descriptor\n");
            return 1;
        }

        // Read value from pin
        if (read(mq7_pin_fd, buffer, BUFFER_SIZE) == -1) {
            printf("Error reading MQ-7 pin value\n");
            return 1;
        }

        // Convert value to integer
        value = atoi(buffer);

        // Calculate voltage based on ADC value (assuming 1.8V Vref)
        voltage = (float)value / 4096 * 1.8;

        // Calculate CO concentration in ppm using a conversion factor of 60mV/ppm (from datasheet)
        // co_ppm = voltage / 0.06;
	co_ppm = 1;

        // Print result
        //printf("CO Concentration: %f ppm\n", co_ppm);

        // Close file descriptor
        close(mq7_pin_fd);

        fprintf(fp,"%d, %d, %.2f, %.2f, %d\n", co2, voc, humidity, temperature, 1);
	if (co2>1000) {
		gpio68_fd = open(GPIO_PATH "/gpio68/value", O_WRONLY);
    		write(gpio68_fd, "0", strlen("0"));
    		close(gpio68_fd);

    		gpio44_fd = open(GPIO_PATH "/gpio44/value", O_WRONLY);
    		write(gpio44_fd, "0", strlen("0"));
    		close(gpio44_fd);

    		gpio26_fd = open(GPIO_PATH "/gpio26/value", O_WRONLY);
    		write(gpio26_fd, "1", strlen("1"));
    		close(gpio26_fd);
        } else if (co_ppm > 0) {
        	gpio68_fd = open(GPIO_PATH "/gpio68/value", O_WRONLY);
    		write(gpio68_fd, "0", strlen("0"));
    		close(gpio68_fd);

    		gpio44_fd = open(GPIO_PATH "/gpio44/value", O_WRONLY);
    		write(gpio44_fd, "0", strlen("0"));
    		close(gpio44_fd);

    		gpio26_fd = open(GPIO_PATH "/gpio26/value", O_WRONLY);
    		write(gpio26_fd, "1", strlen("1"));
    		close(gpio26_fd);
        } else {
		    gpio68_fd = open(GPIO_PATH "/gpio68/value", O_WRONLY);
    		write(gpio68_fd, "1", strlen("1"));
    		close(gpio68_fd);

    		gpio44_fd = open(GPIO_PATH "/gpio44/value", O_WRONLY);
    		write(gpio44_fd, "0", strlen("0"));
    		close(gpio44_fd);

    		gpio26_fd = open(GPIO_PATH "/gpio26/value", O_WRONLY);
    		write(gpio26_fd, "0", strlen("0"));
    		close(gpio26_fd);
	} 
        fclose(fp);
        usleep(500000);
    }
    return 0;
}
