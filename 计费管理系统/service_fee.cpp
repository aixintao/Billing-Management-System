#include "global.h"
#include "data_card.h"
#include "data_vip.h"
#include "data_log.h"

extern char cardfile[];

extern int moneyleast;
extern int moneymost;

void charge_msg(char* name, double money) {
	printf("�ѳɹ��� %s �г�ֵ %.2lfԪ��\n", name, money);
}

void refund_msg(char* name, double balance) {
	printf("�ѳɹ�ע���� %s��������� %.2lfԪ����ע����գ�\n", name, balance);
}

int recharge() {
	struct Card* temp = (struct Card*)malloc(sizeof(struct Card));
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		return -1;
	}
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		free(temp);
		return -1;
	}
	char* name = getstr_letnum("�����뿨���ƽ���������0-16�ַ���ֻ����Ӣ����ĸ�����֣�ֱ�ӻس����ȫ������", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		free(temp);
		free(psw);
		return 0;
	}

	int charge = getint("��������Ҫ��ֵ�Ŀ�����ţ�����0ֱ�ӷ��أ���", 0, cardcnt);

	if (charge > 0) {
		Card c = get_card_name(list, charge, name);
		if (getpsw(c.password, psw) == -1) {
			//��¼��Ϣ����
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 20; //������������
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
		int money = getcurrency("�������ֵ��", moneyleast, moneymost);
		c.balance += money;
		double m = money / 100.0; //ʵ��Ǯ����Ԫ��
		int lvl = get_vip_level(m);
		charge_msg(c.id, m);
		{
			//��¼��Ϣ����
			logNode* list = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 3; //��ֵ
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
			printf("���ѳɹ�����Ϊ ");
			color(get_vip_color(lvl)); 
			get_vip_title(lvl, psw);
			printf("%s", psw);
			color(); 
			printf("��\n");
			//��¼��Ϣ����
			logNode* list = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, name);
			info->type = 30; //������Ա
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
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		return -1;
	}
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		free(temp);
		return -1;
	}

	char* name = getstr_letnum("�����뿨���ƽ���������0-16�ַ���ֻ����Ӣ����ĸ�����֣�ֱ�ӻس����ȫ������", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		free(temp);
		free(psw);
		return 0;
	}

	int refund = getint("��������Ҫ�˷ѵĿ�����ţ�����0ֱ�ӷ��أ���", 0, cardcnt);
	if (refund > 0) {
		Card c = get_card_name(list, refund, name); 
		if (c.balance < 0) {
			printf("�ÿ���Ƿ�ѣ����Ƚ���Ƿ�����˷ѣ�\n");
			cardsave(list);
			free(name);
			free(temp);
			free(psw);
			return 0;
		}
		if (c.on) {
			printf("�ÿ��ϻ��У������ϻ����˷ѣ�\n");
			cardsave(list);
			free(name);
			free(temp);
			free(psw);
			return 0;
		}
		if (getpsw(c.password, psw) == -1) {
			//��¼��Ϣ����
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, c.id);
			info->type = 20; //������������
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
		//��¼��Ϣ����
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, c.id);
		info->type = 4; //�˷�
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