#include "dir.h"
//#ifndef DIR_H
//#define DIR_H
//#endif

char * get_cwd(){
	char * cwd=(char *)malloc(1000*sizeof(char));
	getcwd(cwd, sizeof(cwd));
	return cwd;
}	


char * get_parent(char * path){
	char* parent = strdup(path); 
    int len=strlen(path);
    int found=-1,i;
    for(i=0; i<len; i++)
  	{
  		if(parent[i]=='/')  
		{
  			found=i;	
 		}
	}
    if(found != -1)
  	{ 		
  		i=found;
  	
	  	while(i < len)
  		{
  			parent[i]='\0';
			i++;  
		}
	}
	return parent;
}	


//Returns Absolute Path
char * get_path (char path[]){
	char *abs_path=NULL;
	if( path[0] == '/' || path[0] == '~'){
		abs_path=(char *)malloc(strlen(path)+1);
		strcpy(abs_path,path);
	}	
	else if( path[0] != '/' || path[0] != '~' ){
	    char cwd[1000];
	    char * curr=".";
	    char * parent="..";
	    getcwd(cwd, sizeof(cwd));
	    abs_path=(char *)malloc(strlen(path)+strlen(cwd)+1);
	    if((strcmp(path,curr)==0) && (strlen(path) == 1)){
	        strcpy(abs_path,cwd);
	    }   
	    else if((strcmp(path,parent)==0) && (strlen(path) == 2)){
			strcpy(abs_path,cwd);
	        strcat(abs_path,"/..");
	    }
	    else{
			strcpy(abs_path,cwd);
			strcat(abs_path,"/");
	        strcat(abs_path,path);
	    }
	} 
	return abs_path;
}


//Open Directory
DIR * open_dir(char * path){
	DIR * dir;
	dir=opendir(path);
	if (dir == NULL) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        printf("Error : Failed to open input directory - %s\n", strerror(errno));
        fflush(stdout);
        return NULL;
    }
    else
        return dir;
}	

//Close Directory
int close_dir(DIR * dir){
	int ret=closedir(dir);
	if(ret == 1)
	{
		fprintf(stderr, "Error : Failed to close directory - %s\n", strerror(errno));
        printf("Error : Failed to close directory - %s\n", strerror(errno));
        fflush(stdout);
        return 1;
	}
	return 0;
}	

//List Directory-Files list
vector<char *> print_dir(DIR * dir,char * path){	
	struct dirent * dir_struct;
	vector <char *> file_list;
	printf("\033[31;1m");
	printf("Folder-%s\n",path);
	printf("\033[0G");
	struct  winsize w = get_termsize();
	for(int i=0;i<w.ws_col;i++)
	    printf(".");
	printf("\033[0m");
	printf("\033[1B");
	printf("\033[33;1m\033[0GName\033[35GPermissions\033[49GUser\033[59GGroup\033[69GSize\033[79GModified Time\n\033[0m\033[1B");
	fflush(stdout);
	printf("\033[0G");
	printf("\0337");
	//printf("\033[0G\033[7h.");
	printf("\033[0G\033[36m.\033[0m");
	file_list.push_back(".");
	//printf("%s\n",path);
	print_details(path);
	//printf("\033[0G\033[7h..");
	printf("\033[0G\033[36m..\033[0m");
	file_list.push_back("..");
	char * parent_path=get_parent(path);
	//printf("%s\n",parent_path);
	print_details(parent_path);
    fflush(stdout);
    free(parent_path);
    int file_count=2;
 	while ((dir_struct = readdir(dir))) 
    {    char * file_path=(char *)malloc((strlen(path)+500)*sizeof(char));
	     strcpy(file_path,path);
	     strcat(file_path,"/");
	     strcat(file_path,dir_struct->d_name);
         if (!strcmp (dir_struct->d_name, ".")){
            continue;
         }
         if (!strcmp (dir_struct->d_name, "..")){
            continue;
         }
         if(dir_struct->d_name[0]=='.')  //Not displaying hidden files
            continue;
         if(dir_struct->d_type==DT_DIR){
			file_count++;
            //printf("\033[0G\033[7h\033[36m%s/\033[0m",dir_struct->d_name);
            printf("\033[0G\033[36m%s/\033[0m",dir_struct->d_name);
            file_list.push_back(dir_struct->d_name);
	        //printf("%s\n",file_path);
	        print_details(file_path);
            fflush(stdout);
         }
         if(dir_struct->d_type==DT_REG){
			file_count++;
            //printf("\033[0G\033[7h%s",dir_struct->d_name);
            printf("\033[0G%s",dir_struct->d_name);
            file_list.push_back(dir_struct->d_name);
	        //printf("%s\n",file_path);
	        print_details(file_path);
            fflush(stdout);
         }
         free(file_path);
    }
    printf("\0338");
    fflush(stdout);
    return file_list;
}	


//List Directory-Details of files
void print_details(char * path)
{
	struct stat file_stat;
	if (stat(path, &file_stat) == -1) {
        perror("stat");
        //exit(EXIT_FAILURE);
        return;
    }

    //Permissions
    printf("\033[35G");
    printf( (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf( (file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXOTH) ? "x" : "-");
    
    //user group
    printf("\033[49G");
    printf("%s  ",getpwuid(file_stat.st_uid)->pw_name);
    printf("\033[59G%s",getgrgid(file_stat.st_gid)->gr_name);
    
    //size
    double size=(double)file_stat.st_size;
    char * fsize=readable_size(size);
    printf("\033[69G%s",fsize);
    
    //last modified time
    printf("\033[79G%s",ctime(&file_stat.st_mtime));
    
	fflush(stdout);
	return;
}	

bool isDir(char * path)
{
	struct stat file_stat;
	if (stat(path, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if(S_ISDIR(file_stat.st_mode)){
		return 1;
	}
	return 0;
}	


bool isRegfile(char * path)
{
	struct stat file_stat;
	if (stat(path, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if(S_ISREG(file_stat.st_mode)){
		return 1;
	}
	return 0;
}

char* readable_size(double file_size) {
    int i = 0;
    char * formated_size=(char*)malloc(1000*sizeof(char));
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (file_size > 1024) {
        file_size /= 1024;
        i++;
    }
    sprintf(formated_size, "%.*f %s", i, file_size, units[i]);
    return formated_size;
}

//Test
/*
int main(int argc, char **argv){
	char * path;
	//read command line args
	if(argc == 2)
	    path=argv[1];
	if(argc == 1)
	    path=".";
	else{    
	    printf("Usage:./dir path \n If no path specified pwd is taken\n");
	    return 1;
    }
    
    char * abs_path=get_path(path);	
    printf("abs_path : %s\n",abs_path);
	DIR * dir=open_dir(abs_path);
	 if(dir != NULL)
	 {
	      print_dir(dir,abs_path);   
     }
     free(abs_path);
     close_dir(dir);
     return 0;
}
*/
