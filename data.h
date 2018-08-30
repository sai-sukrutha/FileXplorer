#include <stdio.h>
#include <iostream>
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

vector <file_data> files_list;

extern map <string,vector<file_data>> file_structure;  //name,files_list

stack <string> UndoStack;
stack <string> RedoStack;
