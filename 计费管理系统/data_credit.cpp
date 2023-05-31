#include "global.h"
#include "data_credit.h"

typedef creditNode node;
typedef blacklist data;


//��������
node* build() {
	node* head = (node*)malloc(sizeof(node));
	if (!head) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return head;
	}
	head->next = 0;
	return head;
}

//����Ԫ��
void add(node* head, data* data) {
	node* go = (node*)malloc(sizeof(node));
	if (!go) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return;
	}
	go->data = *data;
	go->next = 0;
	node* p = head;
	while (p->next) {
		p = p->next;
	}
	p->next = go;
}

//ɾ����cnt��Ԫ��
void del_cnt(creditNode* head, int cnt) {
	if (cnt <= 0) return;
	int count = 1;
	node* p = head;
	while (p->next && count < cnt) {
		p = p->next;
		count++;
	}
	if (!(p->next)) return;
	node* temp = p->next;
	p->next = p->next->next;
	free(temp);
}

//ɾ����cnt����������Ԫ��
void del_cnt_name(creditNode* head, int cnt, const char* name) {
	if (cnt <= 0) return;
	int count = 0;
	node* p = head;
	if (p->next && !strstr(p->next->data.id, name)) count++;
	while (p->next && count < cnt) {
		p = p->next;
		if (p->next && !strstr(p->next->data.id, name)) count++;
	}
	if (!(p->next)) return;
	node* temp = p->next;
	p->next = p->next->next;
	free(temp);
}

//ɾ��ȫ��
void del_all(node* head) {
	node* p = head, * pnow;
	while (p->next) {
		pnow = p->next;
		p->next = p->next->next;
		free(pnow);
	}
	free(head);
}

//blacklist�Ų���
int search_exist(creditNode* head, const char* name) {
	node* p = head->next;
	while (p) {
		if (!strcmp(p->data.id, name)) return 1;
		p = p->next;
	}
	return 0;
}

creditNode* blacklistload() {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* head = build();
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return 0;
	}
	fopen_s(&fp, blacklistfile, "r+b");
	if (!fp) {
		fopen_s(&fp, blacklistfile, "w+b");
		if (!fp) {
			colorerr();
			printf("�޷���%s����������Ƿ���:(\n", blacklistfile);
			color();
			exit(256);
		}
		else {
			fclose(fp);
			fopen_s(&fp, blacklistfile, "r+b");
			if (!fp) {
				colorerr();
				printf("�޷���%s����������Ƿ���:(\n", blacklistfile);
				color();
				exit(256);
			}
		}
	}
	rewind(fp);
	while (1) {
		fread_s(temp, sizeof(data), sizeof(data), 1, fp);
		if (feof(fp)) break;
		add(head, temp);
	}
	fclose(fp);
	return head;
}

void blacklistsave(creditNode* head) {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* p = head->next;
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return;
	}
	fopen_s(&fp, blacklistfile, "w+b");
	if (!fp) {
		colorerr();
		printf("�޷���%s����������Ƿ���:(\n", blacklistfile);
		color();
		exit(256);
	};
	while (p) {
		fwrite(&(p->data), sizeof(data), 1, fp);
		p = p->next;
	}
	fclose(fp);
	del_all(head);
	return;
}

void blacklistadd(creditNode* head, struct blacklist* n) {
	add(head, n);
}

//������˵�
void showblacklistmenu() {
	printf("��� �������绰����\n");
}

//���blacklist���ݣ��ṹ�壬��ţ�
void showblacklist(struct blacklist* c, int index) {
	printf("%4d ", index);
	printf("%-12s\n", c->id);
}

int cnt_all(creditNode* head) {
	creditNode* p = head->next;
	int cnt = 0;
	while (p) {
		cnt++;
		p = p->next;
	}
	return cnt;
}

int show_all(creditNode* list) {
	creditNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!cnt) showblacklistmenu();
		cnt++;
		showblacklist(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޺�������Ϣ��\n");
	return cnt;
}

int show_all_name(creditNode* list, const char* name) {
	creditNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!strstr(p->data.id, name)) {
			p = p->next;
			continue;
		}
		if (!cnt) showblacklistmenu();
		cnt++;
		showblacklist(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޺�������Ϣ��\n");
	return cnt;
}

void change_blacklist(creditNode* list, int pos, blacklist* n) {
	creditNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		if (cnt == pos) {
			p->data = *n;
			return;
		}
		p = p->next;
	}
}

void change_blacklist_name(creditNode* list, int pos, blacklist* n, const char* name) {
	creditNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!strstr(p->data.id, name)) {
			p = p->next;
			continue;
		}
		cnt++;
		if (cnt == pos) {
			p->data = *n;
			return;
		}
		p = p->next;
	}
}

blacklist get_blacklist(creditNode* list, int pos) {
	creditNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		if (cnt == pos) {
			return p->data;
		}
		p = p->next;
	}
}