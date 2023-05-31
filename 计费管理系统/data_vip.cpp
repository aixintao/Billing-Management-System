#include "global.h"
#include "data_card.h"
#include "data_vip.h"

typedef vipNode node;
typedef struct vip data;

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
void del_cnt(vipNode* head, int cnt) {
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
	//�޸Ĳ����Ŀ���Ϣ������޸ģ�
	CardNode* list = cardload();
	CardNode* q = list;
	while (q->next) {
		if (q->next->data.vip >= cnt) {
			q->next->data.vip--;
		};
		q = q->next;
	}
	cardsave(list);
	free(temp);
}

//ɾ����cnt����������Ԫ��
void del_cnt_name(node* head, int cnt, const char* name) {
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

//vip�Ų���
int search_exist(vipNode* head, const char* name) {
	node* p = head->next;
	while (p) {
		if (!strcmp(p->data.id, name)) return 1;
		p = p->next;
	}
	return 0;
}

vipNode* vipload() {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* head = build();
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return 0;
	}
	fopen_s(&fp, vipfile, "r+b");
	if (!fp) {
		fopen_s(&fp, vipfile, "w+b");
		if (!fp) {
			colorerr();
			printf("�޷���%s����������Ƿ���:(\n", vipfile);
			color();
			exit(256);
		}
		else {
			fclose(fp);
			fopen_s(&fp, vipfile, "r+b");
			if (!fp) {
				colorerr();
				printf("�޷���%s����������Ƿ���:(\n", vipfile);
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

void vipsave(vipNode* head) {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* p = head->next;
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return;
	}
	fopen_s(&fp, vipfile, "w+b");
	if (!fp) {
		colorerr();
		printf("�޷���%s����������Ƿ���:(\n", vipfile);
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

void vipadd(vipNode* head, struct vip* n) {
	add(head, n);
}

//������˵�
void showvipmenu() {
	printf("��� ��Ա��        �����ۿ�  ��������\n");
}

//���vip���ݣ��ṹ�壬��ţ�
void showvip(struct vip* c, int index) {
	color(c->color);
	printf("%4d ", index);
	printf("%-12s", c->id);
	if (c->discount == 100) printf("    ���ۿ�  ");
	else if (c->discount % 10 == 0) printf("       %1d��  ", c->discount / 10);
	else printf("      %02d��  ", c->discount);
	printf("һ���Գ�ֵ��%5dԪ\n", c->gap);
	color();
}

int cnt_all(vipNode* head) {
	vipNode* p = head->next;
	int cnt = 0;
	while (p) {
		cnt++;
		p = p->next;
	}
	return cnt;
}

int show_all(vipNode* list) {
	vipNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!cnt) showvipmenu();
		cnt++;
		showvip(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޻�Ա��Ϣ��\n");
	return cnt;
}

int show_all_name(vipNode* list, const char* name) {
	vipNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!strstr(p->data.id, name)) {
			p = p->next;
			continue;
		}
		if (!cnt) showvipmenu();
		cnt++;
		showvip(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޻�Ա��Ϣ��\n");
	return cnt;
}

void change_vip(vipNode* list, int pos, vip* n) {
	vipNode* p = list->next;
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

void change_card_name(vipNode* list, int pos, vip* n, const char* name) {
	vipNode* p = list->next;
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

vip get_vip(vipNode* list, int pos) {
	vipNode* p = list->next;
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

vip get_vip_name(vipNode* list, int pos, const char* name) {
	vipNode* p = list->next;
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

int get_vip_color(int index) {
	if (index <= 0)  return 7;
	node* list = vipload();
	int tot = cnt_all(list);
	if (index > tot) {
		vipsave(list);
		return 7;
	}
	vipNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		if (cnt == index) {
			int c = p->data.color;
			vipsave(list);
			return c;
		}
		p = p->next;
	}
	vipsave(list);
	return 7;
}

void get_vip_title(int index ,char* str) {
	if (index <= 0) {
		strcpy_s(str, 16, "��");
		return; 
	}
	node* list = vipload();
	int tot = cnt_all(list);
	if (index > tot) {
		vipsave(list);
		strcpy_s(str, 16, "��Ч��Ա");
		return;
	}
	vipNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		if (cnt == index) {
			strcpy_s(str, 16, p->data.id);
			vipsave(list);
			return;
		}
		p = p->next;
	}
	vipsave(list);
}

int get_vip_discount(int index) {
	if (index <= 0)  return 100;
	node* list = vipload();
	int tot = cnt_all(list);
	if (index > tot) {
		vipsave(list);
		return 100;
	}
	vipNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		if (cnt == index) {
			int dis = p->data.discount;
			vipsave(list);
			return dis;
		}
		p = p->next;
	}
	vipsave(list);
	return 100;
}

int get_vip_level(double money) {
	if (money < 0)  return 0;
	node* list = vipload();
	vipNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		if (p->data.gap > money) {
			vipsave(list);
			return cnt-1;
		}
		p = p->next;
	}
	vipsave(list);
	return cnt;
}

void insert(vipNode* head, vip* v) {
	vipNode* p = head;
	int cnt = 0;
	while (p->next) {
		cnt++;
		if (p->next->data.gap > v->gap || (p->next->data.gap == v->gap && p->next->data.discount < v->discount)) break;
		p = p->next;
	}
	if (!p->next) add(p, v);
	else {
		vipNode* temp = (vipNode*)malloc(sizeof(vipNode));
		if (!temp) {
			colorerr();
			printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
			color();
			return;
		}
		temp->data = *v;
		temp->next = p->next;
		p->next = temp;
		//�޸Ĳ����Ŀ���Ϣ������޸ģ�
		//gap/discount��ͬ�������
		int tiecnt = 0;
		vipNode* o = head;
		while (o->next && o->next->data.gap <= v->gap) {
			if (o->next->data.gap == v->gap && o->next->data.discount == v->discount) tiecnt++;
			o = o->next;
		}
		//
		cnt -= tiecnt - 1;
		CardNode *list = cardload();
		CardNode* q = list;
		while (q->next) {
			if (q->next->data.vip >= cnt) {
				q->next->data.vip++;
			};
			q = q->next;
		}
		cardsave(list);
	}
}

//�޸ĺ�ά��˳��
void change_stable(vipNode* head, int count, vip* v) {
	del_cnt(head, count);
	insert(head, v);
}
