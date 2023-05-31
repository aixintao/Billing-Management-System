#include "global.h"
#include "data_standard.h"
#include "data_vip.h"

typedef standardNode node;
typedef struct standard data;

//建立链表
node* build() {
	node* head = (node*)malloc(sizeof(node));
	if (!head) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return head;
	}
	head->next = 0;
	return head;
}

//增加元素
void add(node* head, data* data) {
	node* go = (node*)malloc(sizeof(node));
	if (!go) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
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

//删除第cnt个元素
void del_cnt(node* head, int cnt) {
	if (cnt <= 0) return;
	int count = 1;
	node* p = head;
	while (p->next && count < cnt) {
		p = p->next;
		count++;
	}
	if (!p->next) return;
	node* temp = p->next;
	p->next = p->next->next;
	free(temp);
}

//删除第cnt个符合条件元素
void del_cnt_name(node* head, int cnt, const char* name) {
	if (cnt <= 0) return;
	int count = 0;
	node* p = head;
	if (p->next && strstr(p->next->data.name, name)) count++;
	while (p->next && count < cnt) {
		p = p->next;
		if (p->next && strstr(p->next->data.name, name)) count++;
	}
	if (!(p->next)) return;
	node* temp = p->next;
	p->next = p->next->next;
	free(temp);
}

//删除第cnt个元素（仅做删除标记，保留数据）
void del_cnt_saved(standardNode* head, int cnt) {
	if (cnt <= 0) return;
	int count = 0;
	standardNode* p = head;
	while (p && count < cnt) {
		p = p->next;
		if (!p->data.del) count++;
	}
	if (!p) return;
	p->data.del = 1;
}

//删除第cnt个符合条件元素（仅做删除标记，保留数据）
void del_cnt_name_saved(standardNode* head, int cnt, const char* name) {
	if (cnt <= 0) return;
	int count = 0;
	standardNode* p = head;
	if (p->next && strstr(p->next->data.name, name) && !p->next->data.del) count++;
	while (p->next && count < cnt) {
		p = p->next;
		if (p->next && strstr(p->next->data.name, name) && !p->next->data.del) count++;
	}
	if (!(p->next)) return;
	p->next->data.del = 1;
}

//删除全部
void del_all(node* head) {
	node* p = head, * pnow;
	while (p->next) {
		pnow = p->next;
		p->next = p->next->next;
		free(pnow);
	}
	free(head);
}

void showstandardmenu() {
	printf("当前计费标准（不计折扣）：\n");
}

void showstandard(struct standard* s,int index) {
	printf("%2d) ", index);
	if (/*admin*/0) printf("%.1lf分钟后，%.2lf元/%d分钟；（%.3lf元每分钟）\n", s->gap, s->money, s->minute, s->rate);
	else printf("%.1lf分钟后，%.2lf元/%d分钟；\n", s->gap, s->money, s->minute);
}

void shownone() {
	printf("目前暂无计费标准！\n");
}

int cnt_all(standardNode* head) {
	standardNode* p = head->next;
	int cnt = 0;
	while (p) {
		if (p->data.del) {
			p = p->next;
			continue;
		}
		cnt++;
		p = p->next;
	}
	return cnt;
}

int show_all(standardNode* head){
	standardNode* p = head->next;
	int cnt = 0;
	while (p) {
		if (p->data.del) {
			p = p->next;
			continue;
		}
		if (!cnt) showstandardmenu();
		cnt++;
		showstandard(&p->data, cnt);
		p = p->next;
	}
	if (!cnt) shownone();
	return cnt;
}

standardNode* standardload() {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* head = build();
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return 0;
	}
	fopen_s(&fp, standardfile, "r+");
	if (!fp) {
		fopen_s(&fp, standardfile, "w+");
		if (!fp) {
			colorerr();
			printf("无法打开%s！请检查软件是否损坏:(\n", standardfile);
			color();
			exit(256);
		}
		else {
			fclose(fp);
			fopen_s(&fp, standardfile, "r+");
			if (!fp) {
				colorerr();
				printf("无法打开%s！请检查软件是否损坏:(\n", standardfile);
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

void standardsave(standardNode* head) {
	FILE* fp;
	data* temp = (data*)malloc(sizeof(data));
	node* p = head->next;
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return;
	}
	fopen_s(&fp, standardfile, "w+");
	if (!fp) {
		colorerr();
		printf("无法打开%s！请检查软件是否损坏:(\n", standardfile);
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

standard get_standard(standardNode* list, int pos) {
	standardNode* p = list->next;
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

void insert(standardNode* head, standard* s) {
	standardNode* p = head;
	while (p->next) {
		if (p->next->data.gap > s->gap) break;
		p = p->next;
	}
	if (!p->next) add(p, s);
	else {
		standardNode* temp = (standardNode*)malloc(sizeof(standardNode));
		if (!temp) {
			colorerr();
			printf("无法分配内存！请检查内存剩余空间\n");
			color();
			return;
		}
		temp->data = *s;
		temp->next = p->next;
		p->next = temp;
	}
}

//直接修改
void change(standardNode* head, int count, standard* s) {
	standardNode* p = head;
	int cnt = 0;
	while (p) {
		if (cnt >= count) break;
		p = p->next;
		cnt++;
	}
	if (p) {
		p->data = *s;
	}
}

//修改后维持顺序
void change_stable(standardNode* head, int count,standard* s) {
	del_cnt_saved(head, count);
	insert(head, s);
}

//计算收费金额
//参数：上下机时间、会员等级 返回值：花费钱数（分）
int calcost(time_t on ,time_t off,int vip) {
	long long sec = off - on;
	if (!sec) return 0;
	standardNode* list = standardload();
	int cnt = cnt_all(list);
	if (!cnt) {
		colorerr();
		printf("无计费标准！将按免费处理！\n");
		color();
		standardsave(list);
		return 0;
	}
	standardNode* p = list->next;
	//钱数 （元）
	double res = 0.0;
	//时间（分钟）
	double t = sec / 60.0;
	//上一个标准的开始计费时间
	double lastgap = 0;
	//上一个标准的分钟数
	double lastmin = 1;
	//上一个标准的单位计费
	double lastmoney = 0;
	while (p) {
		if (p->data.del) {
			p = p->next;
			continue;
		}
		if (p->data.gap >= t) break;
		//该档到上一档的时间部分
		double avail = p->data.gap - lastgap;
		res += ceil(avail / lastmin) * lastmoney;
		lastmin = p->data.minute;
		lastmoney = p->data.money;
		lastgap = p->data.gap;
		p = p->next;
	}
	double avail = t - lastgap;
	res += ceil(avail / lastmin) * lastmoney;
	standardsave(list);
	int dis = get_vip_discount(vip);
	res *= (dis / 100.0);
	int cent = int(res * 100.0);
	return cent;
}