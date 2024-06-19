/**
General functions for files, date/time, writing logfiles, etc.
*/

#include <cstdio>
#include <string.h>
#include <string>
#include <dirent.h>
#include <cstdarg>
#include <fstream>
#include <cctype>

#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

using namespace std;


/// File functions
long getFileSize( const char * szFileName );
bool fileExists(const char* fileName);
bool fileExists(const string& fileName);
bool directoryExists( const char* path );
bool directoryExists( const string& path );
//string getStartPath();

/// Date/time functions
string getCurrentDatetime();

/// Logfile functions
bool writeLog(const string& text);
bool writeLog(const char* text, ...);


/// String functions
string toString(const int value);
string toString(const long value);
string lcase(const string& str);
string ucase(const string& str);
string triml(const string& s);
string trimr(const string& s);
string trim(const string& s);
bool compare_nocase (string first, string second);

/// other functions
int hex2dec(const char c);
void beep();


#endif
