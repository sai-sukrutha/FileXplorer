#include "dir.h"

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
int print_dir(DIR * dir,char * path){	
	struct dirent * dir_struct;
	char * temp=NULL;
	file_data temp_struct;
	//printf("%s\n",path);
	//struct  winsize w = get_termsize();
	//for(int i=0;i<w.ws_col;i++)
	//printf("\033[0G------------------------------------------\n\033[1B");
	fflush(stdout);
	printf("\033[0G");
	printf("\0337");
	printf("\033[0G\033[7h.");
	temp_struct.name=".";
	temp=get_path(".");
	temp_struct=print_details(temp,temp_struct);
	files_list.push_back(temp_struct);
    printf("\033[0G\033[7h..");
    temp_struct.name="..";
	temp=get_path("..");
	temp_struct=print_details(temp,temp_struct);
	files_list.push_back(temp_struct);
    fflush(stdout);
    int file_count=2;
 	while ((dir_struct = readdir(dir))) 
    {
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
            printf("\033[0G\033[7h\033[36m%s/\033[0m",dir_struct->d_name);
            temp_struct.name=dir_struct->d_name;
	        temp=get_path(dir_struct->d_name);
	        temp_struct=print_details(temp,temp_struct);
	        files_list.push_back(temp_struct);
            fflush(stdout);
         }
         if(dir_struct->d_type==DT_REG){
			file_count++;
            printf("\033[0G\033[7h%s",dir_struct->d_name);
            temp_struct.name=dir_struct->d_name;
	        temp=get_path(dir_struct->d_name);
	        temp_struct=print_details(temp,temp_struct);
	        files_list.push_back(temp_struct);
            fflush(stdout);
         }
    }
    file_structure[path]=files_list;
    free(temp);
    printf("\0338");
    fflush(stdout);
    return file_count;
}	


//List Directory-Details of files
file_data print_details(char * path,file_data temp_struct)
{
	temp_struct.path=path;
	struct stat file_stat;
	if (stat(path, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    //Permissions
    string perm;
    printf("\033[25G");
    //printf( (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    perm.append((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    //printf( (file_stat.st_mode & S_IRUSR) ? "r" : "-");
    perm.append((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    //printf( (file_stat.st_mode & S_IWUSR) ? "w" : "-");
    perm.append((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    //printf( (file_stat.st_mode & S_IXUSR) ? "x" : "-");
    perm.append((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    //printf( (file_stat.st_mode & S_IRGRP) ? "r" : "-");
    perm.append((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    //printf( (file_stat.st_mode & S_IWGRP) ? "w" : "-");
    perm.append((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    //printf( (file_stat.st_mode & S_IXGRP) ? "x" : "-");
    perm.append((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    //printf( (file_stat.st_mode & S_IROTH) ? "r" : "-");
    perm.append((file_stat.st_mode & S_IROTH) ? "r" : "-");
    //printf( (file_stat.st_mode & S_IWOTH) ? "w" : "-");
    perm.append((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    //printf( (file_stat.st_mode & S_IXOTH) ? "x" : "-");
    perm.append((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf("%s",perm);
    temp_struct.permissions=perm;
    
    //user group
    printf("\033[37G");
    printf("%s  ",getpwuid(file_stat.st_uid)->pw_name);
    temp_struct.user=getpwuid(file_stat.st_uid)->pw_name;
    printf("\033[47G%s",getgrgid(file_stat.st_gid)->gr_name);
    temp_struct.group=getgrgid(file_stat.st_gid)->gr_name;
    
    //size
    //to-do :Convert to readable
    printf("\033[57G%lld",(long long) file_stat.st_size);
    temp_struct.size=(long long)file_stat.st_size;
    
    //last modified time
    printf("\033[77G%s",ctime(&file_stat.st_mtime));
    temp_struct.mtime=ctime(&file_stat.st_mtime);
    
	fflush(stdout);
	return temp_struct;
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
