#pragma once

typedef struct loglist {
	struct loglist* next;
	struct log data;
} logNode;

struct log* log_new();

void showcardmenu();

logNode* logload();
void logsave(logNode* head);

int search_exist(logNode* head, const char* name);

//show zone return count
int show_all(logNode* list);

int show_all_name(logNode* list, const char* name);
int show_all_cardname_time(logNode* list, const char* name, time_t set, time_t end);
int show_all_name_time(logNode* list, const char* name, time_t set, time_t end);

int cnt_all(logNode* list);

void logadd(logNode* head, struct log* n);


int cnt_all_time_charge(logNode* list, time_t set, time_t end);
int cnt_all_time_refund(logNode* list, time_t set, time_t end);
int cnt_all_time_getoff(logNode* list, time_t set, time_t end);

int show_all_time_charge(logNode* list, time_t set, time_t end);
int show_all_time_refund(logNode* list, time_t set, time_t end);
