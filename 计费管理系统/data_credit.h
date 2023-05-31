#pragma once
#include "model.h"
typedef struct creditlist {
	struct creditlist* next;
	blacklist data;
} creditNode;

creditNode* blacklistload();
void blacklistsave(creditNode* head);

int show_all(creditNode* list);
int show_all_name(creditNode* list, const char* name);

void del_cnt(creditNode* head, int cnt);
void del_cnt_name(creditNode* head, int cnt, const char* name);

//blacklist查重，检测是否已被拉黑
int search_exist(creditNode* head, const char* name);

void change_blacklist(creditNode* list, int pos, blacklist* n);
void change_blacklist_name(creditNode* list, int pos, blacklist* n, const char* name);


void blacklistadd(creditNode* head, struct blacklist* n);

blacklist get_blacklist(creditNode* list, int pos);

