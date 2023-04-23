#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    fd = open("/dev/carbonmonoxide", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open device file");
        exit(EXIT_FAILURE);
    }

    while (1) {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("Failed to read from device file");
            exit(EXIT_FAILURE);
        }

        printf("Data read from device file: %s", buffer);
        
        sleep(1); // Pause for 1 second
    }

    close(fd);

    return 0;
}
