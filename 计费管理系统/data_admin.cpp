#include "global.h"
#include "data_admin.h"

typedef adminNode node;
typedef struct admin data;

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
void del_cnt(node* head, int cnt) {
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
void del_cnt_name(adminNode* head, int cnt, const char* name) {
	if (cnt <= 0) return;
	int count = 0;
	node* p = head;
	if (p->next && strstr(p->next->data.id, name)) count++;
	while (p->next && count < cnt) {
		p = p->next;
		if (p->next && strstr(p->next->data.id, name)) count++;
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

//�˺Ų���
int search_exist(adminNode* head, const char* name) {
	node* p = head->next;
	while (p) {
		if (!strcmp(p->data.id, name)) return 1;
		p = p->next;
	}
	return 0;
}

adminNode* adminload() {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* head = build();
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return 0;
	}
	fopen_s(&fp, adminfile, "r+b");
	if (!fp) {
		fopen_s(&fp, adminfile, "w+b");
		if (!fp) {
			colorerr();
			printf("�޷���%s����������Ƿ���:(\n", adminfile);
			color();
			exit(256);
		}
		else {
			fclose(fp);
			fopen_s(&fp, adminfile, "r+b");
			if (!fp) {
				colorerr();
				printf("�޷���%s����������Ƿ���:(\n", adminfile);
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

void adminsave(adminNode* head) {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* p = head->next;
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return;
	}
	fopen_s(&fp, adminfile, "w+b");
	if (!fp) {
		colorerr();
		printf("�޷���%s����������Ƿ���:(\n", adminfile);
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

void adminadd(adminNode* head, struct admin* n) {
	add(head, n);
}

//������˵�
void showadminmenu() {
	printf("���  ����Ա�˺�  ���ʹ��ʱ��\n");
}

//��������ݣ��ṹ�壬��ţ�
void showadmin(struct admin* c, int index) {
	printf("%4d ", index);
	char title[16];
	printf("%-10s", title);
	printf("%-17s", c->id);
	//printf("%4d ", c->on);
	char time[20];
	time_t t = c->geton;
	if (t) {
		TimeToString(t, time);
		printf("%-20s", time);
	}
	else printf("%-20s", "��δʹ�ù�");
	printf("%-16s\n", c->tel);
	color();
}

int show_all(adminNode* list) {
	adminNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!cnt) showadminmenu();
		cnt++;
		showadmin(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޿���Ϣ��\n");
	return cnt;
}

int cnt_all(adminNode* list) {
	adminNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		p = p->next;
	}
	if (!cnt) printf("�޿���Ϣ��\n");
	return cnt;
}

int show_all_name(adminNode* list, const char* name) {
	adminNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!strstr(p->data.id, name)) {
			p = p->next;
			continue;
		}
		if (!cnt) showadminmenu();
		cnt++;
		showadmin(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޿���Ϣ��\n");
	return cnt;
}

void change_admin(adminNode* list, int pos, struct admin* n) {
	adminNode* p = list->next;
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

void change_admin_name(adminNode* list, int pos, struct admin* n, const char* name) {
	adminNode* p = list->next;
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

struct admin get_admin(adminNode* list, int pos) {
	adminNode* p = list->next;
	int cnt = 0;
	while (p->next) {
		cnt++;
		if (cnt == pos) {
			return p->data;
		}
		p = p->next;
	}
	return p->data;
}

struct admin get_admin_name(adminNode* list, int pos, const char* name) {
	adminNode* p = list->next;
	int cnt = 0;
	while (p->next) {
		if (!strstr(p->data.id, name)) {
			p = p->next;
			continue;
		}
		cnt++;
		if (cnt == pos) {
			return p->data;
		}
		p = p->next;
	}
	return p->data;
}
