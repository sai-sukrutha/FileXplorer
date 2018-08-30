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
#include "data.h"

using namespace std;

char * get_path (char []);
DIR * open_dir(char *);
int close_dir(DIR *);
int print_dir(DIR *,char *);
file_data print_details(char *,file_data);

