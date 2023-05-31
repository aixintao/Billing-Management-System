#pragma once
#include "model.h"

typedef struct standardlist {
	struct standardlist* next;
	struct standard data;
} standardNode;

//double getcost(struct Card* c);

void showstandardmenu();
void showstandard(struct standard* s, int index);
standardNode* standardload();
void standardsave(standardNode* head);
int show_all(standardNode* head);
void insert(standardNode* head, standard* s);
void change_stable(standardNode* head, int count, standard* s);
void del_cnt_saved(standardNode* head, int cnt);
int calcost(time_t on, time_t off,int vip);
standard get_standard(standardNode* list, int pos);


