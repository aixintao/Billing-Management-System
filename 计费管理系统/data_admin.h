#pragma once
#include "model.h"

typedef struct adminlist {
	struct adminlist* next;
	struct admin data;
} adminNode;

adminNode* adminload();

void adminsave(adminNode* head);
int search_exist(adminNode* head, const char* name);

void change_admin_name(adminNode* list, int pos, struct admin* n, const char* name);

struct admin get_admin_name(adminNode* list, int pos, const char* name);
int cnt_all(adminNode* list);
void adminadd(adminNode* head, struct admin* n);
void del_cnt_name(adminNode* head, int cnt, const char* name);


