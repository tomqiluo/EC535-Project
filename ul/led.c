#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define GPIO_PATH "/sys/class/gpio"

int main(int argc, char *argv[])
{
    int gpio68_fd, gpio44_fd, gpio26_fd;

    // Export the GPIO pins
    system("echo 68 > " GPIO_PATH "/export");
    system("echo 44 > " GPIO_PATH "/export");
    system("echo 26 > " GPIO_PATH "/export");

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

    // Unexport the GPIO pins
    system("echo 68 > " GPIO_PATH "/unexport");
    system("echo 44 > " GPIO_PATH "/unexport");
    system("echo 26 > " GPIO_PATH "/unexport");

    return 0;
}
