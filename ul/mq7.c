#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define MQ7_PIN "/sys/class/gpio/gpio67/value"
#define BUFFER_SIZE 100

int main() {
    int mq7_pin_fd;
    char buffer[BUFFER_SIZE];
    int value;
    float voltage;
    float co_ppm;

    // Export GPIO pin for MQ-7 sensor
    system("echo 67 > /sys/class/gpio/export");
    // Set direction of GPIO pin to input
    system("echo in > /sys/class/gpio/gpio67/direction");

    while(1) {
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
        co_ppm = voltage / 0.06;

        // Print result
        printf("CO Concentration: %f ppm\n", co_ppm);

        // Close file descriptor
        close(mq7_pin_fd);

        // Sleep for 1 second
        sleep(1);
    }

    return 0;
}
