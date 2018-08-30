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

int flag=1;
int mode=1; // 1- Normal 0-Command

int fd=open(IN_OUT,O_RDWR );

void usage();
void non_canonical(char *);
void canonical();


int main(int argc, char **argv)
{
	char * path;
	//read command line args
	if(argc == 2)
	    path=argv[1]; 
	else if(argc == 1)
	    path=".";
	else{    
	    usage();
	    return 1;
    }
	struct termios old_tio;
	old_tio=get_mode(fd);
    alter_screen();
    /*
    struct  winsize w=get_termsize();
	for(int i=0;i<w.ws_col;i++)
	    printf("-");
    cout<<"File Explorer"<<endl;
	for(int i=0;i<w.ws_col;i++)
	    printf("-");
	cout<<endl;
	fflush(stdout);
	cout<<"Press ':' to Command Mode and 'Esc' again to Normal Mode"<<endl;
	cout<<"Press 'qq' to quit"<<endl<<endl;
	fflush(stdout);
	*/
	change_mode(1,fd,get_mode(fd));  
    non_canonical(path);
	 
     
	char c;
	while(flag)
	{
		read(fd,&c,1);
		//printf("char is : %c\n",c);
		fflush(stdout);
		if(c == ':' && mode == 1){
		    change_mode(2,fd,get_mode(fd));
		    mode=0;
		    canonical();
		}
		if(c == '\033' && mode == 0){
			change_mode(1,fd,get_mode(fd));
			mode=1;
			non_canonical(".");
		}
		if(c == 'q' && mode == 1)
		    flag=0;
    }	
    exit_alter_screen();
    set_mode(old_tio,fd);
    return 0;	
}


void usage()
{
	const char *use=	"Usage: ./fileXplorer path \n If no path specified pwd is taken\n";
	write(fileno(stderr),use,strlen(use));
	return;
}	


void non_canonical(char * path)
{
	 int n_flag=1;
	 printf("\033[2J");
	 goto_last();
	 printf("Normal Mode");
	 printf("\033[H");fflush(stdout);
	//print dir --- 
	 int file_count=0;
	 char * abs_path=get_path(path);
	 DIR * dir=open_dir(abs_path);
	 if(dir != NULL)
	 {
	      file_count=print_dir(dir,abs_path);
     }
     free(abs_path);
     
     char c;
     int pos=0;
     while(n_flag)
     {
		 read(fd,&c,1);
		 if(c == ':')
		 {
			change_mode(2,fd,get_mode(fd));
		    mode=0;
		    canonical(); 
		    return;
		 }
		 
		 //Arrows
		 if (c == '\033') {    // first value is esc
            read(fd,&c,1);     // skip the [
            read(fd,&c,1);     //A,B,C,D
            switch(c) {
                case 'A':      
                    // Up Arrow
                    pos--;
                    printf("\033[A");fflush(stdout);
                    break;
                case 'B':
                    // Down Arrow
                    if(pos <= file_count){
                        printf("\033[B");fflush(stdout);
				    }
				    pos++;
                    break;
                case 'C':
                    // Right Arrow
                    //to-do
                    break;
                case 'D':
                    // Left Arrow
                    //to-do
                    break;           
            }
        }
        
        if(c == 10){
			//Enter
			/*
			if(isfile){
			      
		    }
		    else if(isDir){
				
			    char * file_path =files_list[pos];
			    non_canonical(file_path);
		    }*/	 
		}	 
		 
		if(c == 'q' ){
		    n_flag=0;
		    return;
		 }
    }
    return;	 
}

void canonical()
{
	int c_flag=1;
    char c;
    goto_last();
    printf("\033[K");fflush(stdout);
    printf(":");fflush(stdout);
    printf("\0337");fflush(stdout);
    
    while(c_flag)
    {
		read(fd,&c,1);
		if(c == '\033' && mode == 0){
			c_flag=0;
			change_mode(1,fd,get_mode(fd));
			mode=1;
			//todo-send path from stack or vector
			
			non_canonical(".");
			return;
	    }
	    
	    if(c == 10){ 		
			printf("\033[A");fflush(stdout);
			//goto_last();
			printf("\0338");fflush(stdout);
			printf("\033[1K");fflush(stdout);
            printf(":");fflush(stdout);
			fflush(stdout);
	    }
	}
	return;	    	
}	
