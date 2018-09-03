#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;


vector <char *> get_words(char * cmnd);
void select_command(char * cmnd,char * orig_path,char * curr_path);
void create_dir(char * dir_name,char * dest_path,char * orig_path,char * curr_path);
void create_file(char * file_name,char * dest_path,char * orig_path,char * curr_path);
void delete_dir(char * dir_path_rel,char * orig_path,char * curr_path);
void delete_file(char * file_path_rel,char * orig_path,char * curr_path);
