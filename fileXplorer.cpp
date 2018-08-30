#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "termOps.h"
#include "dir.h"

#define IN_OUT "/dev/tty"
using namespace std;

int fd=open(IN_OUT,O_RDWR );
void usage();

int main(int argc, char **argv)
{
	char * path;
	//read command line args
	if(argc == 2)
	    path=argv[1];
	if(argc == 1)
	    path=".";
	else{    
	    usage();
	    return 1;
    }
	struct termios old_tio;
	old_tio=get_mode(fd);
    alter_screen();
    struct  winsize w=get_termsize();
	for(int i=0;i<w.ws_col;i++)
	    printf("-");
    cout<<"File Explorer"<<endl;
	for(int i=0;i<w.ws_col;i++)
	    printf("-");
	cout<<endl;
	fflush(stdout);
	cout<<"Press ':' to Command Mode and 'Esc' again to Normal Mode"<<endl;
	cout<<"Press 'q' to quit"<<endl<<endl;
	fflush(stdout);
	int flag=1;
	int mode=1; // 1- Normal 0-Command
	change_mode(1,fd,get_mode(fd));  
    
	 //print dir --- 
	 char * abs_path=get_path(path);
	 DIR * dir=open_dir(abs_path);
	 if(dir != NULL)
	 {
	      print_dir(dir,abs_path);  
     }
     free(abs_path);
     
	char c;
	while(flag)
	{
		read(fd,&c,1);
		//printf("char is : %c\n",c);
		fflush(stdout);
		if(c == ':' && mode == 1){
		    change_mode(2,fd,get_mode(fd));
		    goto_last();
		    printf(":");fflush(stdout);
		    mode=0;
		}
		if(c == '\033' && mode == 0){
			change_mode(1,fd,get_mode(fd));
			mode=1;
		}
		if(c == 'q' )
		    flag=0;
    }	
    exit_alter_screen();
    set_mode(old_tio,fd);
    return 0;	
}


void usage()
{
	const char *use="Usage:./fileXplorer path \n If no path specified pwd is taken\n";
	write(fileno(stderr),use,strlen(use));
	return;
}	
