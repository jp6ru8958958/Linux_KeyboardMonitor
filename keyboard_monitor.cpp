#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<linux/input.h>
#include<string.h>
#include<stdio.h>
using namespace std;

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

int main(void){
    const char *dev = {"/dev/input/by-path/platform-i8042-serio-0-event-kbd"};
    char DellXps13_kbdbacklight[] = {"cd /sys/devices/platform/dell-laptop/leds/dell::kbd_backlight"};
    struct input_event ev;
    ssize_t n;
    int fd;
    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }
    while (1) {
    	n = read(fd, &ev, sizeof ev);
        if(n == (ssize_t)-1){
            if (errno == EINTR){
                continue;
	    }else{
                break;
	    }
        }else if(n != sizeof ev){
            errno = EIO;
            break;
        }
        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2){
		printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
		if(ev.value == 1 || ev.value == 2){
			system("echo 2 > /sys/devices/platform/dell-laptop/leds/dell::kbd_backlight/brightness");
			continue;
		}else{
			sleep(1);
			system("echo 0 > /sys/devices/platform/dell-laptop/leds/dell::kbd_backlight/brightness");
		}
	}
    }
    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return EXIT_FAILURE;
}

