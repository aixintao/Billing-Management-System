#include "global.h"
#include "service_credit.h"
#include "data_card.h"
#include "data_credit.h"
#include "data_log.h"

//�Զ����غ�����
void menu_load() {
	creditNode* blist = blacklistload();
	int T = 2; //��ֹ��˳�������©
	while (T--) {
		CardNode* list = cardload();
		CardNode* p = list->next;
		int cnt = 1;
		while (p) {
			if (p->data.del) {
				p = p->next;
				continue;
			}
			//����ֵ
			if (p->data.credit <= 0) {
				blacklist* cp = (blacklist*)malloc(sizeof(blacklist));
				if (!cp) {
					colorerr();
					printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
					color(); 
					return;
				}
				strcpy_s(cp->id, 24, p->data.tel);
				blacklistadd(blist, cp);
				printf("�� %s ������ֵ���ͣ����ֻ��� %s �������ڣ��������Զ�ע����������%.2lfԪ��\n", p->data.id, p->data.tel, p->data.balance / 100.0);
				//��¼��Ϣ����
				{
					logNode* list = logload();
					struct log* info = log_new();
					strcpy_s(info->id, 20, p->data.id);
					info->type = -20; //����������
					strcpy_s(info->tel, 20, p->data.tel);
					info->amount = p->data.balance;
					info->balance = p->data.balance;
					time(&info->time);
					logadd(list, info);
					logsave(list);
				}
				del_cnt_saved(list, cnt);
			}
			else if (search_exist(blist, p->data.tel)) {
				printf("�� %s ���ֻ��� %s �ѱ����ڣ������Զ�ע����������%.2lfԪ��\n", p->data.id, p->data.tel, p->data.balance / 100.0);
				//��¼��Ϣ����
				{
					logNode* list = logload();
					struct log* info = log_new();
					strcpy_s(info->id, 20, p->data.id);
					info->type = -21; //�ֻ���ʧ�ſ�ע��
					strcpy_s(info->tel, 20, p->data.tel);
					info->amount = p->data.balance;
					info->balance = p->data.balance;
					time(&info->time);
					logadd(list, info);
					logsave(list);
				}
				del_cnt_saved(list, cnt);
			}
			p = p->next;
			cnt++;
		}
		cardsave(list);
	}
	blacklistsave(blist);
}