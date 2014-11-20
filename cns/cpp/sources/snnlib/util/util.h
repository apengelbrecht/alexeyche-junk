#pragma once

#include <snnlib/base.h>


vector<string> split_into(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);

void trim(string &str);

#ifndef PI
    #define PI 3.1415926535897932384626433832795028841971693993751
#endif

double getUnif();
double getUnifBetween(double low, double high);
double getExp(double rate);

double sampleDelay(double gain, double rate);
double getNorm();
string strip_white(const string& input);
string strip_comments(const string& input, const string& delimiters);

