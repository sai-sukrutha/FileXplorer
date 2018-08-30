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
#include "termOps.h"

using namespace std;

char * get_path (char []);
DIR * open_dir(char *);
int close_dir(DIR *);
void print_dir(DIR *,char *);
void print_details(char *);

