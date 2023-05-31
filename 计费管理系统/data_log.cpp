#pragma once
#include "global.h"
// #include "data_vip.h" 日志数据中已经储存了和会员相关的信息
#include "data_log.h"

typedef logNode node;
typedef struct log data;

struct log* log_new() {
	struct log* t = (struct log*)calloc(1, sizeof(struct log));
	if (!t) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		return NULL;
	}
	return t;
}

//建立链表
node* build() {
	node* head = (node*)malloc(sizeof(node));
	if (!head) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
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
	if (!(p->next)) return;
	node* temp = p->next;
	p->next = p->next->next;
	free(temp);
}

//删除第cnt个符合条件元素
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

//卡号匹配
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
		printf("无法分配内存！请检查内存剩余空间\n");
		return 0;
	}
	fopen_s(&fp, logfile, "r+b");
	if (!fp) {
		fopen_s(&fp, logfile, "w+b");
		if (!fp) {
			colorerr();
			printf("无法打开%s！请检查软件是否损坏:(\n", logfile);
			exit(256);
		}
		else {
			fclose(fp);
			fopen_s(&fp, logfile, "r+b");
			if (!fp) {
				colorerr();
				printf("无法打开%s！请检查软件是否损坏:(\n", logfile);
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
		printf("无法分配内存！请检查内存剩余空间\n");
		return;
	}
	fopen_s(&fp, logfile, "w+b");
	if (!fp) {
		colorerr();
		printf("无法打开%s！请检查软件是否损坏:(\n", logfile);
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

//输出表头
void showlogmenu() {
	printf("系统日志：\n");
}

//输出内容（日志系统核心）
void showlog(struct log* c, int index) {
	char time[20];
	TimeToString(c->time, time);
	printf("[%-19s] ",time);
	switch (c->type){
	case 1:
		printf("卡 %s 上机了！", c->id);
		break;
	case 2:
		printf("卡 %s 下机了！消费 %.2lf元，余额 %.2lf元！", c->id, c->amount / 100.0, c->balance / 100.0);
		break;
	case 3:
		printf("卡 %s 充值了 %.2lf元！余额 %.2lf元！", c->id, c->amount / 100.0, c->balance / 100.0);
		break;
	case 4:
		printf("卡 %s 退费并注销了！退费金额：%.2lf元", c->id, c->amount / 100.0);
		break;
	case 10:
		printf("手机号 %s 注册了一张卡：%s, 初始金额: %.2lf元", c->tel, c->id, c->amount / 100.0);
		break;
	case 12:
		printf("卡 %s 修改了密码！", c->id);
		break;
	case 14:
		printf("卡 %s 被删除了！", c->id);
		break;
	case 20:
		printf("卡 %s 尝试登录，但密码错误次数过多！", c->id);
		break;
	case 30:
		printf("卡 %s 由于一次性充值 %.2lf元，晋升为 ", c->id, c->amount / 100.0);
		color(c->vipcolor);
		printf(c->vip);
		color();
		printf("！");
		break;
	//以下为管理员部分
	case -1:
		printf("管理员 %s 登录了！", c->id);
		break;
	case -2:
		printf("管理员 %s 改密码了！", c->id);
		break;
	case -3:
		printf("管理员 %s 退出登录了！", c->id);
		break;
	case -4:
		printf("管理员 %s 被删除了！", c->id);
		break;
	case -10:
		printf("管理员 %s 增加了一条计费标准：\n                      %.1lf分钟后，每%d分钟收费%.2lf元！"			,
			c->id, c->gap / 60.0, c->duration / 60, c->amount / 100.0);
		break;
	case -12:
		printf("管理员 %s 修改了一条计费标准：\n                      %.1lf分钟后，每%d分钟收费%.2lf元 ==> %.1lf分钟后，每%d分钟收费%.2lf元！",
			c->id, c->gap_pre / 60.0, c->duration_pre / 60, c->amount_pre / 100.0, c->gap / 60.0, c->duration / 60, c->amount / 100.0);
		break;
	case -14:
		printf("管理员 %s 删除了一条计费标准：\n                      %.1lf分钟后，每%d分钟收费%.2lf元！",
			c->id, c->gap / 60.0, c->duration / 60, c->amount / 100.0);
		break;
	case -20:
		printf("手机号 %s 因卡 %s 信用值过低，被列入黑名单！该卡余额：%.2lf元", c->tel, c->id, c->balance / 100.0);
		break;
	case -21:
		printf("卡 %s 因手机号 %s 被列入黑名单而注销！该卡余额：%.2lf元", c->tel, c->id, c->balance / 100.0);
		break;
	case -24:
		printf("管理员 %s 将手机号 %s 从黑名单中删除！", c->id, c->tel);
		break;
	case -50:
		printf("管理员 %s 增加了一个会员等级：\n                      ", c->id);
		color(c->vipcolor);
		printf("%s", c->vip);
		color();
		printf("：单次充值达到%d元，折扣%2d%%", c->duration/100, 100 - c->amount);
		break;
	case -52:
		printf("管理员 %s 修改了一个会员等级：\n                      ", c->id);
		color(c->vipcolor_pre);
		printf("%s", c->vip_pre);
		color();
		printf("：单次充值达到%d元，折扣%2d%%", c->duration_pre/100, 100 - c->amount_pre);
		printf(" ==> ");
		color(c->vipcolor);
		printf("%s", c->vip);
		color();
		printf("：单次充值达到%d元，折扣%2d%%", c->duration/100, 100 - c->amount);
		break;
	case -54:
		printf("管理员 %s 删除了一个会员等级：\n                      ", c->id);
		color(c->vipcolor);
		printf("%s", c->vip);
		color();
		printf("：单次充值达到%d元，折扣%2d%%", c->duration/100, 100 - c->amount);
		break;
	case -59:
		printf("管理员 %s 尝试登录，但密码错误次数过多！", c->id);
		break;
	case -998:
		printf("系统启动！");
		break;
	case -999:
		printf("系统关闭！");
		break;
	default:
		colorerr();
		printf("哦哟，崩溃了！发现一条错误日志，可能是日志文件受损:(");
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
	if (!cnt) printf("无日志记录！\n");
	return cnt;
}

//统计日志数
int cnt_all(logNode* list) {
	logNode* p = list->next;
	int cnt = 0;
	while (p) {
		cnt++;
		p = p->next;
	}
	if (!cnt) printf("无日志记录！\n");
	return cnt;
}

//个人日志信息（精确匹配）
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
	if (!cnt) printf("无日志记录！\n");
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
	if (!cnt) printf("无日志记录！\n");
	return cnt;
}

//包括管理员日志
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
	if (!cnt) printf("无日志记录！\n");
	return cnt;
}


//统计充值数据（充值、添加卡均计入）
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
	if (!cnt) printf("无充值记录！\n");
	return res;
}

//统计退费数据
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
	if (!cnt) printf("无退费记录！\n");
	return res;
}

//统计下机数据
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
	if (!cnt) printf("无下机记录！\n");
	return res;
}

void show_charge_menu() {
	printf("序号 卡の账号        充值金额  充值时间\n");
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
	printf("序号 卡の账号        退费金额  退费时间\n");
}

void show_refund(struct log* l, int index) {
	printf("%4d ", index);
	printf("%-17s", l->id);
	printf("%7.2lf  ", l->amount / 100.0);
	char time[20];
	TimeToString(l->time, time);
	printf("%-20s\n", time);
}

//显示充值数据（充值、添加卡均计入）
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
	if (!cnt) printf("无充值记录！\n");
	return res;
}

//显示退费数据
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
	if (!cnt) printf("无退费记录！\n");
	return res;
}

