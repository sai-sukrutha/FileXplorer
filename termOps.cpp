#include "termOps.h"

struct termios get_mode(int fd)
{
	struct termios tio;
	tcgetattr(fd,&tio);
	return tio;
}

void set_mode(struct termios tio,int fd)
{
    tcsetattr(fd,TCSANOW,&tio);
	return;
}	

 
/*Changes Modes from Canonical to Non Canonical
 * 1- Non Canonical 2-Canonical */
void change_mode(int want_key,int fd,struct termios old_tio)
{
	struct termios new_tio;

	if(want_key == 1)
	{
		new_tio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
        new_tio.c_iflag = IGNPAR;
        new_tio.c_oflag = 0;
        new_tio.c_lflag = 0;
        new_tio.c_cc[VTIME]= 1;   /* inter-character timer unused */
        new_tio.c_cc[VMIN] = 1;
     }
     if(want_key == 2)
     {
		new_tio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
		new_tio.c_iflag = IGNPAR | INLCR; 
		new_tio.c_oflag = 0;
		new_tio.c_lflag = ICANON;
		new_tio.c_lflag = ECHO;
		new_tio.c_cc[VINTR]    = 1;     /* Ctrl-c */ 
        new_tio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
        new_tio.c_cc[VERASE]   = 1;     /* del */
        new_tio.c_cc[VKILL]    = 0;     /* @ */
        new_tio.c_cc[VEOF]     = 4;     /* Ctrl-d */
        new_tio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
        new_tio.c_cc[VMIN]     = 3;     /* blocking read until n characters arrives */
        new_tio.c_cc[VSWTC]    = 0;     /* '\0' */
        new_tio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
        new_tio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
        new_tio.c_cc[VSUSP]    = 1;     /* Ctrl-z */
        new_tio.c_cc[VEOL]     = 0;     /* '\0' */
        new_tio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
        new_tio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
        new_tio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
        new_tio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
        new_tio.c_cc[VEOL2]    = 0;     /* '\0' */
	 }

    tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &new_tio);
	return;
}

//To enter Alternate Screen
void alter_screen (){
    printf("\033[?1049h\033[H");
}

//To Exit Alternate Screen
void exit_alter_screen (){
    printf("\033[?1049l");
}

//To get Terminal Size
struct  winsize get_termsize(){
	struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w;
    // lines -- w.ws_row   columns -- w.ws_col
}	

//To go to last line of terminal
void goto_last()
{
	struct  winsize w=get_termsize();
	int row=w.ws_row;
	printf("\033[%d;0H",row);
	fflush(stdout);
	return;
}	

/*
//Test
int main(){
	alter_screen();
	cout<<"Press 'E' to exit";
	fflush(stdout);
	char c;
	int flag=1;
	while(flag)
	{
		//set_mode(1); //Non-canonical
		read(fd,&c,1);
		if(c == 'e' || c == 'E'){
		    flag=0;
		    //set_mode(0);
		}
    }	
    exit_alter_screen();
    //set_mode(0);
    return 0;
}
*/
