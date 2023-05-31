#pragma once

typedef struct Cardlist {
	struct Cardlist* next;
	struct Card data;
} CardNode;

void showcardmenu();

void showcard(struct Card* c, int index);

CardNode* cardload();
void cardsave(CardNode* head);

int search_exist(CardNode* head, const char* name);
void cardadd(CardNode* head, struct Card* n);

//show zone return count
int show_all(CardNode* list);
int show_all_name(CardNode* list, const char* name); 

void del_cnt_saved(CardNode* head, int cnt);
void del_cnt_name_saved(CardNode* head, int cnt, const char* name);

void change_card(CardNode* list, int pos, Card* n);
void change_card_name(CardNode* list, int pos, Card* n, const char* name);

Card get_card(CardNode* list, int pos);
Card get_card_name(CardNode* list, int pos, const char* name);

int cnt_all(CardNode* list);
int cnt_all_saved(CardNode* list);

long long get_sum_balance(CardNode* list);