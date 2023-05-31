#include "global.h"
#include "data_card.h"
#include "data_vip.h"
#include "data_credit.h"
#include "data_log.h"

extern int moneyleast; 
extern int moneymost;

int card_new() {
	char* name = getstr_letnum("������1/4���������¿��˺ţ�8-16�ַ���ֻ����Ӣ����ĸ�����֣���", 8, 16);
	CardNode* list = cardload();
	while (search_exist(list, name)) {
		printf("�ÿ����Ѵ��ڣ�");
		free(name);
		name = getstr_letnum("�������¿��˺ţ�8-16�ַ���ֻ����Ӣ����ĸ�����֣���", 8, 16);
	}
	char* psw = getstr_letnum("������2/4�����������루8-16�ַ���ֻ����Ӣ����ĸ�����֣���", 8, 16);
	char* tel = gettel("������3/4��������绰���루������20�ַ�����");
	int money = getcurrency("������4/4���������ֵ��1�����²��ֽ������ԣ���", moneyleast, moneymost);
	int vip = get_vip_level(money / 100.0);
	//��ȡ������ɣ���ʼ������
	time_t now;
	time(&now);
	struct Card* newcard = (struct Card*)calloc(1,sizeof(struct Card));
	if (!newcard) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
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
		printf("�����Զ�����Ϊ ");
		color(get_vip_color(vip));
		get_vip_title(vip, psw);
		printf("%s", psw);
		color();
		printf("��\n");

	}
	//�ļ���ȡ
	//CardNode* list = cardload();
	cardadd(list, newcard);
	cardsave(list);

	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, name);
		info->type = 10; //��ӿ�
		info->amount = money;
		info->balance = money;
		time(&info->time);
		strcpy_s(info->tel, 24, tel);
		logadd(list, info);
		free(info);
		logsave(list);
	}

	if (newcard->vip) {

		//��¼��Ϣ����
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, name);
		info->type = 30; //������Ա
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
	printf("��ӿ���ɣ�\n");
	free(name);
	free(psw);
	free(newcard);
	return 0;
}

int card_ask() {
	char* name = getstr_letnum("�������ѯ���ţ�0-16�ַ���ֻ����Ӣ����ĸ�����֣�ֱ�ӻس����ȫ������", 0, 16);
	CardNode* list = cardload();
	show_all_name(list, name);
	free(name);
	return 0;
}

int card_delete() {
	struct Card* temp = (struct Card*)malloc(sizeof(struct Card));
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n"); 
		color();
		return -1;
	}
	CardNode* list = cardload();
	int cardcnt = show_all(list);
	int del = getint("��������Ҫɾ���Ŀ�����ţ�����0ֱ�ӷ��أ���", 0, cardcnt);
	if (del > 0) {
		Card c = get_card(list, del);
		del_cnt_saved(list, del);
		//��¼��Ϣ����
		{
			logNode* list = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 14; //ɾ����
			info->amount = c.balance;
			info->balance = c.balance;
			time(&info->time);
			strcpy_s(info->tel, 24,c.tel);
			logadd(list, info);
			free(info);
			logsave(list);
		}
		printf("ɾ������ɣ�\n");
	}
	cardsave(list);
	return 0;
}