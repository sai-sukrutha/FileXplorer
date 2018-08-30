#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

#define BAUDRATE B38400

using namespace std;

struct termios get_mode(int fd);
void set_mode(struct termios tio,int fd);
void change_mode(int want_key,int fd,struct termios old_tio);
void alter_screen();
void exit_alter_screen();
struct winsize get_termsize();
void goto_last();
