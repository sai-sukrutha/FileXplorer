#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "termOps.h"
//#include "fileXplorer.h"
//#ifndef FILEXPLORER_H
//#define FILEXPLORER_H
//#endif
//#ifndef TERMOPS_H
//#define TERMOPS_H
//#endif

using namespace std;


//vector <char *> get_words(char * cmnd);
//void select_command(char * cmnd,char * orig_path,char * curr_path);
void create_dir(char * dir_name,char * dest_path,char * orig_path,char * curr_path);
void create_file(char * file_name,char * dest_path,char * orig_path,char * curr_path);
void delete_dir(char * dir_path_rel,char * orig_path,char * curr_path);
void delete_file(char * file_path_rel,char * orig_path,char * curr_path);
void rename_file(char * old_file,char * new_file,char * orig_path);
void move(vector<char *> arg_list,char * orig_path,char *curr_path);
