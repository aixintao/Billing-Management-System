#include "global.h"
#include "data_card.h"
#include "data_standard.h"
#include "data_log.h"
#include "data_vip.h"

int geton() {
	char* name = getstr_letnum("欢迎光临！请输入卡号进行搜索（0-16字符，只允许英文字母和数字；直接回车输出全部）：", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		return 0;
	}
	int charge = getint("请输入将要上机的卡的序号（输入0直接返回）：", 0, cardcnt);

	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return -1;
	}
	if (charge > 0) {
		Card c = get_card_name(list, charge, name);
		//判断未下机、欠费等异常
		{
			if (c.on == 1) {
				printf("该卡未下机，上机失败！\n");
				cardsave(list);
				free(name);
				return 0;
			}
			else if (c.balance == 0) {
				printf("该卡余额为零，请充值后再上机！\n");
				cardsave(list);
				free(name);
				return 0;
			}
			else if (c.balance < 0) {
				printf("该卡已欠费，请充值后再上机！\n");
				cardsave(list);
				free(name);
				return 0;
			}
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
			return 0;
		}
		c.on = 1;
		time(&c.geton);
		change_card_name(list, charge, &c, name);
		//记录信息部分
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, c.id);
		info->type = 1; //上机
		info->balance = c.balance;
		info->time = c.geton;
		strcpy_s(info->tel, 24, c.tel);
		logadd(list, info);
		logsave(list);

		printf("上机成功！\n");
	}
	cardsave(list);
	free(name);
	return 0;
}

int getoff() {
	char* name = getstr_letnum("欢迎光临！请输入卡号进行搜索（0-16字符，只允许英文字母和数字；直接回车输出全部）：", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		return 0;
	}
	int charge = getint("请输入将要下机的卡的序号（输入0直接返回）：", 0, cardcnt);
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		color();
		return -1;
	}
	if (charge > 0) {
		Card c = get_card_name(list, charge, name);
		if (c.on == 0) {
			printf("该卡未上机，下机失败！\n");
			cardsave(list);
			free(name);
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
			return 0;
		}
		c.on = 0;
		time(&c.getoff);
		int cost = calcost(c.geton, c.getoff, c.vip);
		c.balance -= cost;
		c.usecount++;
		double balance = c.balance / 100.0;
		printf("尊敬的%s, 您消费了 %.2lf元, 余额 %.2lf元！\n", c.id, cost / 100.0, balance);
		int days = (int)(100 / (-balance / ((double)debttime / 100)));
		double hours = (100 / (-balance / ((double)debttime / 100)) - days) * 24.0;
		if (c.balance < 0) printf("您已经透支 %.2lf元！请及时补交欠费，否则将在 %d天%.1lf小时后被拉入黑名单！\n", -balance, days, hours);
		change_card_name(list, charge, &c, name);

		//记录信息部分
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, c.id);
		info->type = 2; //下机
		info->balance = c.balance;
		info->time = c.getoff;
		strcpy_s(info->tel, 24, c.tel);
		info->amount = cost;
		info->duration = (int)(c.getoff - c.geton);
		logadd(list, info);
		logsave(list);

		printf("下机成功！\n");
	}
	cardsave(list);
	free(name);
	return 0;
}

