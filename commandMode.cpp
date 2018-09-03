#include "commandMode.h"

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

void select_command(char * cmnd,char * orig_path,char * curr_path){
	vector <char *> arg_list;
	arg_list=get_words(cmnd);  //Splitting with spaces
	
	if(strcmp(arg_list[0],"create_dir")==0){
	    create_dir(arg_list[1],arg_list[2],orig_path,curr_path);
	    return;
	}
	if(strcmp(arg_list[0],"create_file")==0){
	    create_file(arg_list[1],arg_list[2],orig_path,curr_path);
	    return;
	}
	if(strcmp(arg_list[0],"delete_dir")==0){
	    delete_dir(arg_list[1],orig_path,curr_path);
	    return;
	}
	if(strcmp(arg_list[0],"delete_file")==0){
	    delete_file(arg_list[1],orig_path,curr_path);
	    return;
	}
	return;
}

void create_dir(char * dir_name,char * dest_path,char * orig_path,char * curr_path){
	
	mode_t dmode= S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
	int status;
	char * dir_path=(char *)malloc(1000*sizeof(char));
	if(strcmp(dest_path,".")==0)
	    strcpy(dir_path,curr_path);
	else if (dest_path[0]=='.'){
	    strcpy(dir_path,curr_path);
	    char * rel_path=(char *)malloc(1000*sizeof(char));
	    strncpy(rel_path,dest_path+1,strlen(dest_path));
	    strcat(dir_path,rel_path);
	}  
	else if (dest_path[0]=='~'){
	    strcpy(dir_path,orig_path);
	    char * rel_path=(char *)malloc(1000*sizeof(char));
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
	    strncpy(dir_path,dir_path_rel+1,strlen(dir_path_rel));
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
	
}	

void delete_file(char * file_path_rel,char * orig_path,char * curr_path){
	
	char * file_path=(char *)malloc(1000*sizeof(char));
	int status;
	//Specified Relative path to root of application will be given
	if (file_path_rel[0]=='.'){
	    strcpy(file_path,curr_path);
	    strncpy(file_path,file_path_rel+1,strlen(file_path_rel));
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
}	
