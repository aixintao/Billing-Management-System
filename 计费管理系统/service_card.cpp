#include "global.h"
#include "data_card.h"
#include "data_vip.h"
#include "data_credit.h"
#include "data_log.h"

extern int moneyleast; 
extern int moneymost;

int card_new() {
	char* name = getstr_letnum("（步骤1/4）请输入新卡账号（8-16字符，只允许英文字母和数字）：", 8, 16);
	CardNode* list = cardload();
	while (search_exist(list, name)) {
		printf("该卡号已存在！");
		free(name);
		name = getstr_letnum("请输入新卡账号（8-16字符，只允许英文字母和数字）：", 8, 16);
	}
	char* psw = getstr_letnum("（步骤2/4）请输入密码（8-16字符，只允许英文字母和数字）：", 8, 16);
	char* tel = gettel("（步骤3/4）请输入电话号码（不超过20字符）：");
	int money = getcurrency("（步骤4/4）请输入充值金额（1分以下部分将被忽略）：", moneyleast, moneymost);
	int vip = get_vip_level(money / 100.0);
	//获取数据完成，开始创建卡
	time_t now;
	time(&now);
	struct Card* newcard = (struct Card*)calloc(1,sizeof(struct Card));
	if (!newcard) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return -1;
	}
	newcard->set = now;
	strcpy_s(newcard->id,name);
	strcpy_s(newcard->password, psw);
	strcpy_s(newcard->tel, tel);
	newcard->balance = money;
	newcard->credit = 100;
	newcard->vip = vip;
	if(newcard->vip){
		printf("您已自动晋升为 ");
		color(get_vip_color(vip));
		get_vip_title(vip, psw);
		printf("%s", psw);
		color();
		printf("！\n");

	}
	//文件存取
	//CardNode* list = cardload();
	cardadd(list, newcard);
	cardsave(list);

	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, name);
		info->type = 10; //添加卡
		info->amount = money;
		info->balance = money;
		time(&info->time);
		strcpy_s(info->tel, 24, tel);
		logadd(list, info);
		free(info);
		logsave(list);
	}

	if (newcard->vip) {

		//记录信息部分
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, name);
		info->type = 30; //晋升会员
		info->amount = money;
		info->balance = money;
		info->vipcolor = get_vip_color(newcard->vip);
		get_vip_title(newcard->vip,name);
		strcpy_s(info->vip, name);
		time(&info->time);
		strcpy_s(info->tel, 24, tel);
		logadd(list, info);
		free(info);
		logsave(list);

	}
	printf("添加卡完成！\n");
	free(name);
	free(psw);
	free(newcard);
	return 0;
}

int card_ask() {
	char* name = getstr_letnum("请输入查询卡号（0-16字符，只允许英文字母和数字；直接回车输出全部）：", 0, 16);
	CardNode* list = cardload();
	show_all_name(list, name);
	free(name);
	return 0;
}

int card_delete() {
	struct Card* temp = (struct Card*)malloc(sizeof(struct Card));
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n"); 
		color();
		return -1;
	}
	CardNode* list = cardload();
	int cardcnt = show_all(list);
	int del = getint("请输入想要删除的卡的序号（输入0直接返回）：", 0, cardcnt);
	if (del > 0) {
		Card c = get_card(list, del);
		del_cnt_saved(list, del);
		//记录信息部分
		{
			logNode* list = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 14; //删除卡
			info->amount = c.balance;
			info->balance = c.balance;
			time(&info->time);
			strcpy_s(info->tel, 24,c.tel);
			logadd(list, info);
			free(info);
			logsave(list);
		}
		printf("删除卡完成！\n");
	}
	cardsave(list);
	return 0;
}