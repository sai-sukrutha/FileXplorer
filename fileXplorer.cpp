#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
//#include "fileXplorer.h"
#include "termOps.h"
#include "dir.h"
#include "commandMode.h"
//#ifndef FILEXPLORER_H
//#define FILEXPLORER_H
//#endif

#define IN_OUT "/dev/tty"
using namespace std;

stack <char *> LeftStack;
stack <char *> RightStack;

int flag=1;
int mode=1; // 1- Normal 0-Command
int file_count;
int pos;
vector <char *> file_list;

int fd=open(IN_OUT,O_RDWR );
struct termios old_tio=get_mode(fd);

char * orig_path=(char *)malloc(1000*sizeof(char));
char * curr_path=(char *)malloc(1000*sizeof(char));

void usage();
void print_non_canonical(char *);
void non_canonical(char *);
void canonical();
vector <char *> get_words(char * cmnd);
void select_command(char * cmnd);
void goto_dir(char * path);


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
	
	orig_path=get_path(path);
	strcpy(curr_path,orig_path);
	//if(strcmp(path,".")!=0){
	//    strcat(curr_path,"/");
	//	strcat(curr_path,path);
	//}
    alter_screen();
	change_mode(1,fd,get_mode(fd));  
	LeftStack.push(curr_path);
    print_non_canonical(curr_path);
    non_canonical(curr_path);
	char c;
	while(flag)
	{
		read(fd,&c,1);
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
		if(c == 'q' && mode == 1){
		    exit_alter_screen();
            set_mode(old_tio,fd);
		    flag=0;
		}    
    }
    free(orig_path);
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

void print_non_canonical(char * path){
	pos=0;
	printf("\033[2J");
	printf("\033[H");
	printf("\033[34;1m");
	struct  winsize w=get_termsize();
	for(int i=0;i<w.ws_col;i++)
	    printf("-");
    cout<<"FileXplorer"<<endl;
    printf("\033[0G");
	for(int i=0;i<w.ws_col;i++)
	    printf("-");
	printf("\033[0m");
	cout<<endl;
	fflush(stdout);
	cout<<"\033[0G\033[7hPress ':' for Command Mode , 'Esc' to go back to Normal Mode , ";
	cout<<"\033[7h'q' in Normal Mode to quit";
	fflush(stdout);
	
	 goto_last();
	 printf("Normal Mode");
	 printf("\033[6H");fflush(stdout);
	 
	 char * abs_path=get_path(path);
	 strcpy(curr_path,path);
	 /*
	 if(LeftStack.empty())
	     LeftStack.push(path);
	 else if(strcmp(LeftStack.top(),path)!=0)
	     LeftStack.push(path);
	 */ 
	 DIR * dir=open_dir(path);
	 if(dir != NULL)
	 {
	      file_list=print_dir(dir,path);
     }
     free(abs_path);
     file_count=file_list.size();
     return;
}

void non_canonical(char * path)
{
	 int n_flag=1;
     char c;
     pos=0;
     while(n_flag)
     {   
		 //printf("Filecount:%d  pos:%d ",file_count,pos);
		 read(fd,&c,1);
		 //cout<<c;
		 if(c == ':')
		 {
			change_mode(2,fd,get_mode(fd));
		    mode=0;
		    canonical(); 
		    return;
		 }
		 
		 if(c == '/'){
			// Using / instead of Enter
			//printf("Enter is pressed    ");fflush(stdout);
			char * file=(char *)malloc(1000*sizeof(char)); 
			char * curr=file_list[pos];
			strcpy(file,curr_path);
			strcat(file,"/");
			strcat(file,curr);
			if(isDir(file)){
				strcpy(curr_path,file);
      	        LeftStack.push(file);
			    print_non_canonical(file);
		    }
		    else if(isRegfile(file)){
				//File display
				char * argv[2];
				//printf("The path of file is %s\n",file);fflush(stdout);
                argv[1]=file;
                argv[2]=(char *)NULL;
			    int pid=fork();
			    if(pid == -1)
			        printf("Error:cannot fork :%s\n",strerror(errno));
			    if ( pid == 0){
					execv("/usr/bin/xdg-open",argv);	
			    }
			free(file);
		    }
		}
		 
		 char * temp;
		 //Arrows
		 if (c == '\033') {    // first value is esc
            read(fd,&c,1);     // skip the [
            read(fd,&c,1);     //A,B,C,D
            switch(c) {
                case 'A':      
                    // Up Arrow
                    if(pos > 0){
                        printf("\033[A");fflush(stdout);
                        pos--;
				    }
                    break;
                case 'B':
                    // Down Arrow
                    if(pos <= file_count-2){
                        printf("\033[B");fflush(stdout);
                        pos++;
				    }
                    break;
                case 'C':
                    // Right Arrow
                    if(!RightStack.empty()){
                        temp=RightStack.top();
                        //printf("RS[Top] = %s   ",temp);fflush(stdout);
                        RightStack.pop();
                        LeftStack.push(temp);
                        //printf("Pushing %s into LS   ",temp);fflush(stdout);
                        //if(!LeftStack.empty())
                        print_non_canonical(LeftStack.top());
				    }
				    else
				        //printf("RS is empty    ");
                    break;
                case 'D':
                    // Left Arrow
                    if(!LeftStack.empty()){
                        temp=LeftStack.top();
                        //printf("LS[Top] = %s   ",temp);fflush(stdout);
                        LeftStack.pop();
                        RightStack.push(temp);
                        //printf("Pushing %s into RS   ",temp);fflush(stdout);
                        if(!LeftStack.empty())
                            print_non_canonical(LeftStack.top());
				    }
				    else
				        //printf("LS is empty    ");
                    break;           
            }
        }
        	 	 
        if(c == 'h' || c == 'H'){
			strcpy(curr_path,orig_path);
			LeftStack.push(orig_path);
			print_non_canonical(orig_path);
		}	
		
		if( c == 127 ){  //BackSpace
 			if(strcmp(path,orig_path)!=0){
 			    char * parent_path=get_parent(path);
 			    strcpy(curr_path,parent_path);
 			    LeftStack.push(parent_path);
		        print_non_canonical(parent_path);
		    }
	    }
		if(c == 'q' ){
		    n_flag=0;
		    exit_alter_screen();
            set_mode(old_tio,fd);
            free(orig_path);
	        exit(0);
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
    //printf("\0337");fflush(stdout);
    
    char *buf=(char*)malloc(1000*sizeof(char));
    int ptr=0;
    while(c_flag )
    {
		read(fd,&c,1);
		if(c == '\033' && mode == 0){
			c_flag=0;
			change_mode(1,fd,get_mode(fd));
			mode=1;
			ptr=0;
			free(buf);
			print_non_canonical(curr_path);
			non_canonical(curr_path);
			return;
	    }
	    
	    if(c == 10 || c == 13){ 		
			select_command(buf);
			goto_last();
            printf("\033[2K");fflush(stdout);
            printf(":");fflush(stdout);
			ptr=0;
	    }
	    /*
	    if( c == 127 ){ 
		    printf("\033[D");fflush(stdout);
		    printf("\033[0K");fflush(stdout);
		    ptr--;
		    continue;	
		}	
		*/
	    buf[ptr++]=c;
	}
	free(buf);
	return;	    	
}	

vector <char *> get_words(char * cmnd){
	vector <char *> arg_list;
	char * word;
    word=strtok(cmnd," ");
    while (word!=NULL)
    {
        arg_list.push_back(word);
        word=strtok(NULL," ");
    }
	return arg_list;
}	

void select_command(char * cmnd){
	vector <char *> arg_list;
	arg_list=get_words(cmnd);  //Splitting with spaces
	int len=arg_list.size();
	
	if(strcmp(arg_list[0],"create_dir")==0){
		if(len != 3){
		    printf("Error:Incorrect Arguments Usage:create_dir <dir_name> <destination_path>");fflush(stdout);
		    return;
		}
	    create_dir(arg_list[1],arg_list[2],orig_path,curr_path);
	    return;
	}
	if(strcmp(arg_list[0],"create_file")==0){
		if(len != 3){
		    perror("Error:Incorrect Arguments Usage:create_file <dir_name> <destination_path>");
		    return;
		}
	    create_file(arg_list[1],arg_list[2],orig_path,curr_path);
	    return;
	}
	if(strcmp(arg_list[0],"delete_dir")==0){
		if(len != 2){
		    perror("Error:Incorrect Arguments Usage:delete_dir <directory_path>");
		    return;
		}
	    delete_dir(arg_list[1],orig_path,curr_path);
	    return;
	}
	if(strcmp(arg_list[0],"delete_file")==0){
		if(len != 2){
		    perror("Error:Incorrect Arguments Usage:delete_file <file_path>");
		    return;
		}
	    delete_file(arg_list[1],orig_path,curr_path);
	    return;
	}
	if(strcmp(arg_list[0],"goto")==0){
		if(len != 2){
		    perror("Error:Incorrect Arguments Usage:goto <directory_path>");
		    return;
		}
	    goto_dir(arg_list[1]);
	    return;
	}	
	if(strcmp(arg_list[0],"rename")==0){
		if(len != 3){
		    perror("Error:Incorrect Arguments Usage:rename <old_filename> <new_filename>");
		    return;
		}
		rename_file(arg_list[1],arg_list[2],orig_path);
		return;
	}	
	if(strcmp(arg_list[0],"move")==0){
		if(len < 3){
		    perror("Error:Incorrect Arguments Usage:move <source_file(s)> <directory_directory>");
		    return;
		}
		move(arg_list,orig_path,curr_path);
		return;
	}
	fprintf(stderr,"Command not found");//fflush(stdout);
	return;
}

//goto command
void goto_dir(char * path){
	//Specified Absolute path or '/' will be given
	char * abs_path=(char *)malloc(1000*sizeof(char));
	if(strcmp(path,"/")==0)
	    strcpy(abs_path,orig_path);
	else{
		strcpy(abs_path,orig_path);
		strcat(abs_path,"/");
		strcat(abs_path,path);
    }
	change_mode(1,fd,get_mode(fd));  
	LeftStack.push(abs_path);
    print_non_canonical(abs_path);
    non_canonical(abs_path);
    return;
}	

