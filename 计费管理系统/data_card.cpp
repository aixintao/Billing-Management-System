#include "global.h"
#include "data_card.h"
#include "data_vip.h"

typedef CardNode node;
typedef struct Card data;

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

//ɾ����cnt��Ԫ�أ�����ɾ����ǣ��������ݣ�
void del_cnt_saved(CardNode* head, int cnt) {
	if (cnt <= 0) return;
	int count = 1;
	CardNode* p = head;
	while (p->next && count < cnt) {
		p = p->next;
		if(!p->next->data.del) count++;
	}
	if (!(p->next)) return;
	p->next->data.del = 1;
}

//ɾ����cnt����������Ԫ�أ�����ɾ����ǣ��������ݣ�
void del_cnt_name_saved(CardNode* head, int cnt, const char* name) {
	if (cnt <= 0) return;
	int count = 0;
	CardNode* p = head;
	if (p->next && strstr(p->next->data.id, name) && !p->next->data.del) count++;
	while (p->next && count < cnt) {
		p = p->next;
		if (p->next && strstr(p->next->data.id, name) && !p->next->data.del) count++;
	}
	if (!(p->next)) return;
	p->next->data.del = 1;
}

//ɾ��ȫ��
void del_all(node* head) {
	node* p = head,*pnow;
	while (p->next) {
		pnow = p->next;
		p->next = p->next->next;
		free(pnow);
	}
	free(head);
}

//���Ų���
int search_exist(CardNode* head, const char* name) {
	node* p = head->next;
	while (p) {
		if (!strcmp(p->data.id, name)) return 1;
		p = p->next;
	}
	return 0;
}

CardNode* cardload() {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* head = build();
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return 0;
	}
	fopen_s(&fp, cardfile, "r+b");
	if (!fp) {
		fopen_s(&fp, cardfile, "w+b");
		if (!fp) {
			colorerr();
			printf("�޷���%s����������Ƿ���:(\n", cardfile);
			color();
			exit(256);
		}
		else {
			fclose(fp);
			fopen_s(&fp, cardfile, "r+b");
			if (!fp) {
				colorerr();
				printf("�޷���%s����������Ƿ���:(\n", cardfile);
				color();
				exit(256);
			}
		}
	}
	rewind(fp);
	while (1) {
		fread_s(temp, sizeof(data), sizeof(data), 1, fp);
		//��������ֵ
		if (!temp->del) {
			if (temp->balance < 0){
				time_t now;
				time(&now);
				temp->credit = 100 - (int)((now - temp->getoff) / 86400.0 * ((-temp->balance / 100.0) / (debttime/100)));
				if (temp->credit < 0) temp->credit = 0;
			}
			else {
				temp->credit = 100;
			}
		}
		if (feof(fp)) break;
		add(head, temp);
	}
	fclose(fp);
	return head;
}

void cardsave(CardNode* head) {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* p = head->next;
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return;
	}
	fopen_s(&fp, cardfile, "w+b");
	if (!fp) {
		colorerr();
		printf("�޷���%s����������Ƿ���:(\n", cardfile);
		color();
		exit(256);
	};
	while (p) {
		fwrite(&(p->data),sizeof(data),1,fp);
		p = p->next;
	}
	fclose(fp);
	del_all(head);
	return;
}

void cardadd(CardNode* head, struct Card* n) {
	add(head, n);
}

//������˵�
void showcardmenu() {
	printf("��� ��Ա�ȼ�  �����˺�           ״̬   ���   ����ֵ  ���ʹ��ʱ��        �ֻ�����\n");
}

//��������ݣ��ṹ�壬��ţ�
void showcard(struct Card* c, int index) {
	color(get_vip_color(c->vip));
	printf("%4d ", index);
	char title[16];
	get_vip_title(c->vip, title);
	printf("%-10s", title);
	printf("%-17s", c->id);
	switch (c->on) {
	case 0:
		printf("δ�ϻ� ");
		break;
	case 1:
		printf("���ϻ� ");
		break;
	default:
		printf("  δ֪ ");
		break;
	}
	//printf("%4d ", c->on);
	printf("%8.2lf ", c->balance / 100.0);
	printf("%-8d", c->credit);
	char time[20];
	time_t t = (c->geton > c->getoff) ? c->geton : c->getoff;
	if (t) {
		TimeToString(t, time);
		printf("%-20s", time);
	}
	else printf("%-20s", "��δʹ�ù�");
	printf("%-16s\n", c->tel);
	color();
}

int show_all(CardNode* list) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (p->data.del) {
			p = p->next;
			continue;
		}
		if (!cnt) showcardmenu();
		cnt++;
		showcard(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޿���Ϣ��\n");
	return cnt;
}

//ͳ��δע������
int cnt_all(CardNode* list) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (p->data.del) {
			p = p->next;
			continue;
		}
		cnt++;
		p = p->next;
	}
	if (!cnt) printf("�޿���Ϣ��\n");
	return cnt;
}

//ͳ�ư���������
int cnt_all_saved(CardNode* list) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		p = p->next;
	}
	if (!cnt) printf("�޿���Ϣ��\n");
	return cnt;
}

int show_all_name(CardNode* list, const char* name) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (p->data.del || !strstr(p->data.id, name)) {
			p = p->next;
			continue;
		}
		if (!cnt) showcardmenu();
		cnt++;
		showcard(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("�޿���Ϣ��\n");
	return cnt;
}

void change_card(CardNode* list, int pos, Card* n) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (p->data.del || 0) {
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

void change_card_name(CardNode* list, int pos, Card* n, const char* name) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (p->data.del || !strstr(p->data.id, name)) {
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

Card get_card(CardNode* list, int pos) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p->next) {
		if (p->data.del || 0) {
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

Card get_card_name(CardNode* list, int pos, const char* name) {
	CardNode* p = list->next;
	int cnt = 0;
	while (p->next) {
		if (p->data.del || !strstr(p->data.id, name)) {
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

//������Ч����������λ���֣�
long long get_sum_balance(CardNode* list) {
	CardNode* p = list->next;
	long long res = 0;
	while (p) {
		if (p->data.del) {
			p = p->next;
			continue;
		}
		res += p->data.balance;
		p = p->next;
	}
	return res;
}