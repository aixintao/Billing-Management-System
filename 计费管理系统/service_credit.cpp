#include "global.h"
#include "service_credit.h"
#include "data_card.h"
#include "data_credit.h"
#include "data_log.h"

//自动加载黑名单
void menu_load() {
	creditNode* blist = blacklistload();
	int T = 2; //防止因卡顺序造成遗漏
	while (T--) {
		CardNode* list = cardload();
		CardNode* p = list->next;
		int cnt = 1;
		while (p) {
			if (p->data.del) {
				p = p->next;
				continue;
			}
			//信用值
			if (p->data.credit <= 0) {
				blacklist* cp = (blacklist*)malloc(sizeof(blacklist));
				if (!cp) {
					colorerr();
					printf("无法分配内存！请检查内存剩余空间\n");
					color(); 
					return;
				}
				strcpy_s(cp->id, 24, p->data.tel);
				blacklistadd(blist, cp);
				printf("卡 %s 因信用值过低，其手机号 %s 将被拉黑，卡将被自动注销！卡内余额：%.2lf元！\n", p->data.id, p->data.tel, p->data.balance / 100.0);
				//记录信息部分
				{
					logNode* list = logload();
					struct log* info = log_new();
					strcpy_s(info->id, 20, p->data.id);
					info->type = -20; //黑名单拉入
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
				printf("卡 %s 的手机号 %s 已被拉黑，将被自动注销！卡内余额：%.2lf元！\n", p->data.id, p->data.tel, p->data.balance / 100.0);
				//记录信息部分
				{
					logNode* list = logload();
					struct log* info = log_new();
					strcpy_s(info->id, 20, p->data.id);
					info->type = -21; //手机号失信卡注销
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