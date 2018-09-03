#include "commandMode.h"
//#ifndef COMMANDMODE_H
//#define COMMANDMODE_H
//#endif


void create_dir(char * dir_name,char * dest_path,char * orig_path,char * curr_path){
	
	mode_t dmode= S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
	int status;
	char * dir_path=(char *)malloc(1000*sizeof(char));
	if(strcmp(dest_path,".")==0)
	    strcpy(dir_path,curr_path);
	else if (dest_path[0]=='.'){
	    strcpy(dir_path,curr_path);
	    char rel_path[strlen(dest_path)];
	    strncpy(rel_path,dest_path+1,strlen(dest_path));
	    strcat(dir_path,rel_path);
	}  
	else if (dest_path[0]=='~'){
	    strcpy(dir_path,orig_path);
	    char rel_path[strlen(dest_path)];
	    strncpy(rel_path,dest_path+1,strlen(dest_path));
	    strcat(dir_path,rel_path);
	}    
	else
	    strcpy(dir_path,dest_path);
	strcat(dir_path,"/");
	strcat(dir_path,dir_name);
	//printf("Directory path is %s\n",dir_path);
    status = mkdir(dir_path, dmode);
	if(status == -1)
	   printf("Error in Directory Creation : %s\n",strerror(errno));
	free(dir_path);
	return;
}	


void create_file(char * file_name,char * dest_path,char * orig_path,char * curr_path){
	
	mode_t fmode= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	char * file_path=(char *)malloc(1000*sizeof(char));
	int fd;
	if(strcmp(dest_path,".")==0)
	    strcpy(file_path,curr_path);
	else if (dest_path[0]=='.'){
	    strcpy(file_path,curr_path);
	    strncpy(file_path,dest_path+1,strlen(dest_path));
	}
	else if (dest_path[0]=='~'){
	    strcpy(file_path,orig_path);
	    strncpy(file_path,dest_path+1,strlen(dest_path));
	}    
	else
	    strcpy(file_path,dest_path);
	strcat(file_path,"/");
	strcat(file_path,file_name);
	//printf("File path is %s\n",file_path);
    fd = creat(file_path, fmode);
	if(fd == -1)
	   printf("Error in File Creation : %s\n",strerror(errno));
	free(file_path);   
	return;
}	

void delete_dir(char * dir_path_rel,char * orig_path,char * curr_path){
	
	char * dir_path=(char *)malloc(1000*sizeof(char));
	int status;
	//Specified Relative path to root of application will be given
	if (dir_path_rel[0]=='.'){
	    strcpy(dir_path,curr_path);
	    char temp[strlen(dir_path_rel)];
	    strncpy(temp,dir_path_rel+1,strlen(dir_path_rel));
	    strcat(dir_path,temp);
	}
	else{
	    strcpy(dir_path,curr_path); 
	    strcat(dir_path,"/");
	    strcat(dir_path,dir_path_rel);
    }
	//printf("Directory path is %s\n",dir_path);
    status = rmdir(dir_path);
	if(status == -1)
	   printf("Error in Directory Deletion : %s\n",strerror(errno));  
	free(dir_path);
	return;
}	

void delete_file(char * file_path_rel,char * orig_path,char * curr_path){
	
	char * file_path=(char *)malloc(1000*sizeof(char));
	int status;
	//Specified Relative path to root of application will be given
	if (file_path_rel[0]=='.'){
	    strcpy(file_path,curr_path);
	    char temp[strlen(file_path_rel)];
	    strncpy(temp,file_path_rel+1,strlen(file_path_rel));
	    strcat(file_path,temp);
	}
	else{
	    strcpy(file_path,curr_path); 
	    strcat(file_path,"/");
	    strcat(file_path,file_path_rel);
    }
	//printf("File path is %s\n",file_path);
    status = remove(file_path);
	if(status != 0)
	   printf("Error in File Deletion : %s\n",strerror(errno));
	free(file_path);
	return;
}	

//goto_dir in fileXplorer.cpp



void rename_file(char * old_file,char * new_file,char * orig_path){
	//Assuming absolute paths from root of application
	char * abs_path_old=(char *)malloc(1000*sizeof(char));
	char * abs_path_new=(char *)malloc(1000*sizeof(char));
	//Old file path
	strcpy(abs_path_old,orig_path);
	strcat(abs_path_old,"/");
	strcat(abs_path_old,old_file);
	//New file path
	strcpy(abs_path_new,orig_path);
	strcat(abs_path_new,"/");
	strcat(abs_path_new,new_file);
    
	int status;
	status=rename(abs_path_old,abs_path_new);
	if(status != 0)
	    printf("Error in Renaming file : %s\n",strerror(errno));
	return;
}

void move(vector<char *> arg_list,char * orig_path,char *curr_path){
	int n=arg_list.size();
	char *dest_path=arg_list[n-1];
	char * dest_file_path=(char *)malloc(1000*sizeof(char));
	for(int i=1;i<n-1;i++)
	{
		strcpy(dest_file_path,dest_path);
	    strcat(dest_file_path,"/");
	    strcat(dest_file_path,arg_list[i]);
	    rename_file(arg_list[i],dest_file_path,orig_path);
    }
    return;
}	 
