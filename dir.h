#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <errno.h>
#include <ctime>
#include <pwd.h>
#include <grp.h>
#include <malloc.h>
#include <vector>
#include <map>
#include <stack>

using namespace std;


typedef struct{
	string name;
	string path;
	string permissions;
	string user;
	string group;
	long long int size;
	string mtime;
}file_data;

char * get_cwd();
char * get_parent(char *);
char * get_path (char []);
DIR * open_dir(char *);
int close_dir(DIR *);
vector<char *> print_dir(DIR *,char *);
void print_details(char *);
bool isDir(char *);
bool isRegfile(char *);
char* readable_size(double size);
