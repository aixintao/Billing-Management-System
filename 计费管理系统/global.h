#pragma once
//global headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "model.h"
extern char menufile[];
extern char submenufile[];

extern char cardfile[];
extern char standardfile[];
extern char vipfile[];
extern char blacklistfile[];
extern char adminfile[];
extern char logfile[];

extern int passwordlimit;
extern double debttime;
extern int admin;
extern char adminid[];
//eat unvalid input
void eatline();
void input_msg(int lower, int upper);
int getopt(int lower, int upper);
double getlf(double lower, double upper);
int getint(const char* msg, int lower, int upper);
double getdouble(const char* msg, double lower, double upper);
char* getstr_letnum(const char* msg, int lower, int upper);
void input_error();
void input_error(int left, int right);
void input_error(double left, double right);
void TimeToString(time_t time, char* str);
time_t StringToTime(char* timestr);
FILE* openfp(FILE* fp, const char* filename, const char* mode);
int getpsw(const char* st, char* buf);
int getcurrency(const char* msg, double lower, double upper);
char* gettel(const char* msg);
void color(int index = 7); 
void colorerr();
void show_all_color();
int check_admin();

char* getstr(const char* msg, int lower, int upper);

time_t getday(const char* msg);
void getday_tips();
