#include "global.h"
#include "data_card.h"
#include "data_vip.h"
#include "data_log.h"

extern char cardfile[];

extern int moneyleast;
extern int moneymost;

void charge_msg(char* name, double money) {
	printf("已成功向 %s 中充值 %.2lf元！\n", name, money);
}

void refund_msg(char* name, double balance) {
	printf("已成功注销卡 %s，返还余额 %.2lf元，请注意查收！\n", name, balance);
}

int recharge() {
	struct Card* temp = (struct Card*)malloc(sizeof(struct Card));
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		return -1;
	}
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		free(temp);
		return -1;
	}
	char* name = getstr_letnum("请输入卡名称进行搜索（0-16字符，只允许英文字母和数字；直接回车输出全部）：", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		free(temp);
		free(psw);
		return 0;
	}

	int charge = getint("请输入想要充值的卡的序号（输入0直接返回）：", 0, cardcnt);

	if (charge > 0) {
		Card c = get_card_name(list, charge, name);
		if (getpsw(c.password, psw) == -1) {
			//记录信息部分
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 20; //卡密码错误过多
			time(&info->time);
			logadd(llist, info);
			logsave(llist);
			free(info);
			cardsave(list);
			free(name);
			free(temp);
			free(psw);
			return 0;
		}
		int money = getcurrency("请输入充值金额：", moneyleast, moneymost);
		c.balance += money;
		double m = money / 100.0; //实际钱数（元）
		int lvl = get_vip_level(m);
		charge_msg(c.id, m);
		{
			//记录信息部分
			logNode* list = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 3; //充值
			info->amount = money;
			info->balance = c.balance;
			time(&info->time);
			strcpy_s(info->tel, 24, c.tel);
			time(&info->time);
			logadd(list, info);
			logsave(list);
			free(info);
		}
		if (lvl > c.vip) {
			c.vip = lvl;
			printf("您已成功晋升为 ");
			color(get_vip_color(lvl)); 
			get_vip_title(lvl, psw);
			printf("%s", psw);
			color(); 
			printf("！\n");
			//记录信息部分
			logNode* list = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, name);
			info->type = 30; //晋升会员
			info->amount = money;
			info->balance = c.balance;
			info->vipcolor = get_vip_color(lvl);
			get_vip_title(lvl, name);
			strcpy_s(info->vip, name);
			time(&info->time);
			strcpy_s(info->tel, 24, c.tel);
			logadd(list, info);
			free(info);
			logsave(list);
		}
		change_card_name(list, charge, &c, name);

	}
	cardsave(list);
	free(temp);
	free(name);
	free(psw);
	return 0;
}

int refund() {
	struct Card* temp = (struct Card*)malloc(sizeof(struct Card));
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		return -1;
	}
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		free(temp);
		return -1;
	}

	char* name = getstr_letnum("请输入卡名称进行搜索（0-16字符，只允许英文字母和数字；直接回车输出全部）：", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		free(temp);
		free(psw);
		return 0;
	}

	int refund = getint("请输入想要退费的卡的序号（输入0直接返回）：", 0, cardcnt);
	if (refund > 0) {
		Card c = get_card_name(list, refund, name); 
		if (c.balance < 0) {
			printf("该卡已欠费，请先缴清欠款再退费！\n");
			cardsave(list);
			free(name);
			free(temp);
			free(psw);
			return 0;
		}
		if (c.on) {
			printf("该卡上机中，请先上机再退费！\n");
			cardsave(list);
			free(name);
			free(temp);
			free(psw);
			return 0;
		}
		if (getpsw(c.password, psw) == -1) {
			//记录信息部分
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 20; //卡密码错误过多
			time(&info->time);
			logadd(llist, info);
			logsave(llist);
			cardsave(list);
			free(name);
			free(temp);
			free(psw);
			return 0;
		}
		int bal = c.balance;
		double m = c.balance / 100.0;
		refund_msg(c.id, m);
		del_cnt_name_saved(list, refund, name);
		//记录信息部分
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, c.id);
		info->type = 4; //退费
		info->amount = bal;
		info->balance = 0;
		info->time = c.getoff;
		strcpy_s(info->tel, 24, c.tel);
		time(&info->time);
		logadd(list, info);
		logsave(list);
		free(info);
	}
	cardsave(list);
	free(name);
	return 0;
}