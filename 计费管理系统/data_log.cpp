#pragma once
#include "global.h"
// #include "data_vip.h" ��־�������Ѿ������˺ͻ�Ա��ص���Ϣ
#include "data_log.h"

typedef logNode node;
typedef struct log data;

struct log* log_new() {
	struct log* t = (struct log*)calloc(1, sizeof(struct log));
	if (!t) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		return NULL;
	}
	return t;
}

//��������
node* build() {
	node* head = (node*)malloc(sizeof(node));
	if (!head) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
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

//����ƥ��
int search_exist(logNode* head, const char* name) {
	node* p = head->next;
	while (p) {
		if (!strcmp(p->data.id, name)) return 1;
		p = p->next;
	}
	return 0;
}

logNode* logload() {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* head = build();
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		return 0;
	}
	fopen_s(&fp, logfile, "r+b");
	if (!fp) {
		fopen_s(&fp, logfile, "w+b");
		if (!fp) {
			colorerr();
			printf("�޷���%s����������Ƿ���:(\n", logfile);
			exit(256);
		}
		else {
			fclose(fp);
			fopen_s(&fp, logfile, "r+b");
			if (!fp) {
				colorerr();
				printf("�޷���%s����������Ƿ���:(\n", logfile);
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

void logsave(logNode* head) {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* p = head->next;
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		return;
	}
	fopen_s(&fp, logfile, "w+b");
	if (!fp) {
		colorerr();
		printf("�޷���%s����������Ƿ���:(\n", logfile);
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

void logadd(logNode* head, struct log* n) {
	add(head, n);
}

//�����ͷ
void showlogmenu() {
	printf("ϵͳ��־��\n");
}

//������ݣ���־ϵͳ���ģ�
void showlog(struct log* c, int index) {
	char time[20];
	TimeToString(c->time, time);
	printf("[%-19s] ",time);
	switch (c->type){
	case 1:
		printf("�� %s �ϻ��ˣ�", c->id);
		break;
	case 2:
		printf("�� %s �»��ˣ����� %.2lfԪ����� %.2lfԪ��", c->id, c->amount / 100.0, c->balance / 100.0);
		break;
	case 3:
		printf("�� %s ��ֵ�� %.2lfԪ����� %.2lfԪ��", c->id, c->amount / 100.0, c->balance / 100.0);
		break;
	case 4:
		printf("�� %s �˷Ѳ�ע���ˣ��˷ѽ�%.2lfԪ", c->id, c->amount / 100.0);
		break;
	case 10:
		printf("�ֻ��� %s ע����һ�ſ���%s, ��ʼ���: %.2lfԪ", c->tel, c->id, c->amount / 100.0);
		break;
	case 12:
		printf("�� %s �޸������룡", c->id);
		break;
	case 14:
		printf("�� %s ��ɾ���ˣ�", c->id);
		break;
	case 20:
		printf("�� %s ���Ե�¼�����������������࣡", c->id);
		break;
	case 30:
		printf("�� %s ����һ���Գ�ֵ %.2lfԪ������Ϊ ", c->id, c->amount / 100.0);
		color(c->vipcolor);
		printf(c->vip);
		color();
		printf("��");
		break;
	//����Ϊ����Ա����
	case -1:
		printf("����Ա %s ��¼�ˣ�", c->id);
		break;
	case -2:
		printf("����Ա %s �������ˣ�", c->id);
		break;
	case -3:
		printf("����Ա %s �˳���¼�ˣ�", c->id);
		break;
	case -4:
		printf("����Ա %s ��ɾ���ˣ�", c->id);
		break;
	case -10:
		printf("����Ա %s ������һ���Ʒѱ�׼��\n                      %.1lf���Ӻ�ÿ%d�����շ�%.2lfԪ��"			,
			c->id, c->gap / 60.0, c->duration / 60, c->amount / 100.0);
		break;
	case -12:
		printf("����Ա %s �޸���һ���Ʒѱ�׼��\n                      %.1lf���Ӻ�ÿ%d�����շ�%.2lfԪ ==> %.1lf���Ӻ�ÿ%d�����շ�%.2lfԪ��",
			c->id, c->gap_pre / 60.0, c->duration_pre / 60, c->amount_pre / 100.0, c->gap / 60.0, c->duration / 60, c->amount / 100.0);
		break;
	case -14:
		printf("����Ա %s ɾ����һ���Ʒѱ�׼��\n                      %.1lf���Ӻ�ÿ%d�����շ�%.2lfԪ��",
			c->id, c->gap / 60.0, c->duration / 60, c->amount / 100.0);
		break;
	case -20:
		printf("�ֻ��� %s �� %s ����ֵ���ͣ���������������ÿ���%.2lfԪ", c->tel, c->id, c->balance / 100.0);
		break;
	case -21:
		printf("�� %s ���ֻ��� %s �������������ע�����ÿ���%.2lfԪ", c->tel, c->id, c->balance / 100.0);
		break;
	case -24:
		printf("����Ա %s ���ֻ��� %s �Ӻ�������ɾ����", c->id, c->tel);
		break;
	case -50:
		printf("����Ա %s ������һ����Ա�ȼ���\n                      ", c->id);
		color(c->vipcolor);
		printf("%s", c->vip);
		color();
		printf("�����γ�ֵ�ﵽ%dԪ���ۿ�%2d%%", c->duration/100, 100 - c->amount);
		break;
	case -52:
		printf("����Ա %s �޸���һ����Ա�ȼ���\n                      ", c->id);
		color(c->vipcolor_pre);
		printf("%s", c->vip_pre);
		color();
		printf("�����γ�ֵ�ﵽ%dԪ���ۿ�%2d%%", c->duration_pre/100, 100 - c->amount_pre);
		printf(" ==> ");
		color(c->vipcolor);
		printf("%s", c->vip);
		color();
		printf("�����γ�ֵ�ﵽ%dԪ���ۿ�%2d%%", c->duration/100, 100 - c->amount);
		break;
	case -54:
		printf("����Ա %s ɾ����һ����Ա�ȼ���\n                      ", c->id);
		color(c->vipcolor);
		printf("%s", c->vip);
		color();
		printf("�����γ�ֵ�ﵽ%dԪ���ۿ�%2d%%", c->duration/100, 100 - c->amount);
		break;
	case -59:
		printf("����Ա %s ���Ե�¼�����������������࣡", c->id);
		break;
	case -998:
		printf("ϵͳ������");
		break;
	case -999:
		printf("ϵͳ�رգ�");
		break;
	default:
		colorerr();
		printf("ŶӴ�������ˣ�����һ��������־����������־�ļ�����:(");
		color();
		break;
	}
	printf("\n");
}

int show_all(logNode* list) {
	logNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!cnt) showlogmenu();
		cnt++;
		showlog(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("����־��¼��\n");
	return cnt;
}

//ͳ����־��
int cnt_all(logNode* list) {
	logNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		p = p->next;
	}
	if (!cnt) printf("����־��¼��\n");
	return cnt;
}

//������־��Ϣ����ȷƥ�䣩
int show_all_name(logNode* list, const char* name) {
	logNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (!strcmp(p->data.id, name)) {
			p = p->next;
			continue;
		}
		if (!cnt) showlogmenu();
		cnt++;
		showlog(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("����־��¼��\n");
	return cnt;
}


int show_all_cardname_time(logNode* list, const char* name, time_t set, time_t end) {
	logNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (p->data.time<set || p->data.time>end || (!strstr(p->data.id, name))) {
			p = p->next;
			continue;
		}
		if (!cnt) showlogmenu();
		cnt++;
		if (p->data.type>=0 || p->data.type < -997) showlog(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("����־��¼��\n");
	return cnt;
}

//��������Ա��־
int show_all_name_time(logNode* list, const char* name, time_t set, time_t end) {
	logNode* p = list->next;
	int cnt = 0;
	while (p) {
		if (p->data.time<set || p->data.time>end || (!strstr(p->data.id, name) && p->data.id[0] == 0 && name[0] != 0)) {
			p = p->next;
			continue;
		}
		if (!cnt) showlogmenu();
		cnt++;
		showlog(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) printf("����־��¼��\n");
	return cnt;
}


//ͳ�Ƴ�ֵ���ݣ���ֵ����ӿ������룩
int cnt_all_time_charge(logNode* list, time_t set, time_t end) {
	logNode* p = list->next;
	int res = 0;
	int cnt = 0;
	while (p) {
		if ((p->data.type != 3 && p->data.type != 10) || p->data.time<set || p->data.time>end) {
			p = p->next;
			continue;
		}
		cnt++;
		res += p->data.amount;
		p = p->next;
	}
	if (!cnt) printf("�޳�ֵ��¼��\n");
	return res;
}

//ͳ���˷�����
int cnt_all_time_refund(logNode* list, time_t set, time_t end) {
	logNode* p = list->next;
	int res = 0;
	int cnt = 0;
	while (p) {
		if ((p->data.type != 4) || p->data.time<set || p->data.time>end) {
			p = p->next;
			continue;
		}
		cnt++;
		res += p->data.amount;
		p = p->next;
	}
	if (!cnt) printf("���˷Ѽ�¼��\n");
	return res;
}

//ͳ���»�����
int cnt_all_time_getoff(logNode* list, time_t set, time_t end) {
	logNode* p = list->next;
	int res = 0;
	int cnt = 0;
	while (p) {
		if ((p->data.type != 2) || p->data.time<set || p->data.time>end) {
			p = p->next;
			continue;
		}
		cnt++;
		res += p->data.amount;
		p = p->next;
	}
	if (!cnt) printf("���»���¼��\n");
	return res;
}

void show_charge_menu() {
	printf("��� �����˺�        ��ֵ���  ��ֵʱ��\n");
}

void show_charge(struct log* l,int index) {
	printf("%4d ", index);
	printf("%-17s", l->id);
	printf("%7.2lf  ", l->amount / 100.0);
	char time[20];
	TimeToString(l->time, time);
	printf("%-20s\n", time);
}

void show_refund_menu() {
	printf("��� �����˺�        �˷ѽ��  �˷�ʱ��\n");
}

void show_refund(struct log* l, int index) {
	printf("%4d ", index);
	printf("%-17s", l->id);
	printf("%7.2lf  ", l->amount / 100.0);
	char time[20];
	TimeToString(l->time, time);
	printf("%-20s\n", time);
}

//��ʾ��ֵ���ݣ���ֵ����ӿ������룩
int show_all_time_charge(logNode* list, time_t set, time_t end) {
	logNode* p = list->next;
	int res = 0;
	int cnt = 0;
	while (p) {
		if ((p->data.type != 3 && p->data.type != 10) || p->data.time<set || p->data.time>end) {
			p = p->next;
			continue;
		}
		if (!cnt) show_charge_menu();
		cnt++;
		show_charge(&p->data, cnt);
		res += p->data.amount;
		p = p->next;
	}
	if (!cnt) printf("�޳�ֵ��¼��\n");
	return res;
}

//��ʾ�˷�����
int show_all_time_refund(logNode* list, time_t set, time_t end) {
	logNode* p = list->next;
	int res = 0;
	int cnt = 0;
	while (p) {
		if ((p->data.type != 4) || p->data.time<set || p->data.time>end) {
			p = p->next;
			continue;
		}
		if (!cnt) show_refund_menu();
		cnt++;
		show_refund(&p->data, cnt);
		res += p->data.amount;
		p = p->next;
	}
	if (!cnt) printf("���˷Ѽ�¼��\n");
	return res;
}

