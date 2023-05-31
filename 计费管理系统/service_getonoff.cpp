#include "global.h"
#include "data_card.h"
#include "data_standard.h"
#include "data_log.h"
#include "data_vip.h"

int geton() {
	char* name = getstr_letnum("��ӭ���٣������뿨�Ž���������0-16�ַ���ֻ����Ӣ����ĸ�����֣�ֱ�ӻس����ȫ������", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		return 0;
	}
	int charge = getint("�����뽫Ҫ�ϻ��Ŀ�����ţ�����0ֱ�ӷ��أ���", 0, cardcnt);

	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		return -1;
	}
	if (charge > 0) {
		Card c = get_card_name(list, charge, name);
		//�ж�δ�»���Ƿ�ѵ��쳣
		{
			if (c.on == 1) {
				printf("�ÿ�δ�»����ϻ�ʧ�ܣ�\n");
				cardsave(list);
				free(name);
				return 0;
			}
			else if (c.balance == 0) {
				printf("�ÿ����Ϊ�㣬���ֵ�����ϻ���\n");
				cardsave(list);
				free(name);
				return 0;
			}
			else if (c.balance < 0) {
				printf("�ÿ���Ƿ�ѣ����ֵ�����ϻ���\n");
				cardsave(list);
				free(name);
				return 0;
			}
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
			return 0;
		}
		c.on = 1;
		time(&c.geton);
		change_card_name(list, charge, &c, name);
		//��¼��Ϣ����
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, c.id);
		info->type = 1; //�ϻ�
		info->balance = c.balance;
		info->time = c.geton;
		strcpy_s(info->tel, 24, c.tel);
		logadd(list, info);
		logsave(list);

		printf("�ϻ��ɹ���\n");
	}
	cardsave(list);
	free(name);
	return 0;
}

int getoff() {
	char* name = getstr_letnum("��ӭ���٣������뿨�Ž���������0-16�ַ���ֻ����Ӣ����ĸ�����֣�ֱ�ӻس����ȫ������", 0, 16);
	CardNode* list = cardload();
	int cardcnt = show_all_name(list, name);
	if (!cardcnt) {
		cardsave(list);
		free(name);
		return 0;
	}
	int charge = getint("�����뽫Ҫ�»��Ŀ�����ţ�����0ֱ�ӷ��أ���", 0, cardcnt);
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		color();
		return -1;
	}
	if (charge > 0) {
		Card c = get_card_name(list, charge, name);
		if (c.on == 0) {
			printf("�ÿ�δ�ϻ����»�ʧ�ܣ�\n");
			cardsave(list);
			free(name);
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
			return 0;
		}
		c.on = 0;
		time(&c.getoff);
		int cost = calcost(c.geton, c.getoff, c.vip);
		c.balance -= cost;
		c.usecount++;
		double balance = c.balance / 100.0;
		printf("�𾴵�%s, �������� %.2lfԪ, ��� %.2lfԪ��\n", c.id, cost / 100.0, balance);
		int days = (int)(100 / (-balance / ((double)debttime / 100)));
		double hours = (100 / (-balance / ((double)debttime / 100)) - days) * 24.0;
		if (c.balance < 0) printf("���Ѿ�͸֧ %.2lfԪ���뼰ʱ����Ƿ�ѣ������� %d��%.1lfСʱ�������������\n", -balance, days, hours);
		change_card_name(list, charge, &c, name);

		//��¼��Ϣ����
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, c.id);
		info->type = 2; //�»�
		info->balance = c.balance;
		info->time = c.getoff;
		strcpy_s(info->tel, 24, c.tel);
		info->amount = cost;
		info->duration = (int)(c.getoff - c.geton);
		logadd(list, info);
		logsave(list);

		printf("�»��ɹ���\n");
	}
	cardsave(list);
	free(name);
	return 0;
}

