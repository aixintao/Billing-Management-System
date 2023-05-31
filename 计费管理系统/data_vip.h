#pragma once

typedef struct viplist {
	struct viplist* next;
	struct vip data;
} vipNode;


vipNode* vipload();
void vipsave(vipNode* head);

void insert(vipNode* head, vip* v);
void del_cnt(vipNode* head, int cnt);
int show_all(vipNode* list);

void change_stable(vipNode* head, int count, vip* v);

int get_vip_color(int index);
void get_vip_title(int index, char* str);

int get_vip_discount(int index);
int get_vip_level(double money);

vip get_vip(vipNode* list, int pos);