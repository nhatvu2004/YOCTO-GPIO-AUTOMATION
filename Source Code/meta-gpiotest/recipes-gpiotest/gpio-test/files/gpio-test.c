#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#define GPIO_SET_PIN _IOW('G', 0, int)
#define GPIO_CLEAR_PIN _IOW('G', 1, int)

void delay(int seconds) 
{
    sleep(seconds);
}

int main() 
{
    int fd = open("/dev/gpio-driver", O_RDWR);
    if (fd < 0) 
    {
        perror("Failed to open GPIO driver");
        return 1;
    }

    int pins[] = {17, 27, 22};
    for (int i = 0; i < 3; i++) 
    {
        int pin = pins[i];

        // Bật GPIO
		if (ioctl(fd, GPIO_SET_PIN, &pin)<0)
		{
			fprintf(stdout, "GPIO%d set HIGH failed\n", pin);
		}
		else
		{
			fprintf(stdout, "GPIO%d set HIGH succeeded\n", pin);
		}

        delay(5); // Giữ sáng LED trong 5 giây

        // Tắt GPIO
		if (ioctl(fd, GPIO_CLEAR_PIN, &pin)<0)
		{
			fprintf(stdout, "GPIO%d set LOW failed\n", pin);
		}
		else
		{
			fprintf(stdout, "GPIO%d set LOW succeeded\n", pin);
		}

    }

    close(fd);
    return 0;
}

