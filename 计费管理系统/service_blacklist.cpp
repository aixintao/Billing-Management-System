#include "service_blacklist.h"
#include "data_credit.h"
#include "global.h"
#include "data_log.h"

int blacklist_ask() {
	if (!check_admin()) return 0;
	creditNode* list = blacklistload();
	show_all(list);
	blacklistsave(list);
	return 0;
}

int blacklist_delete() {
	if (!check_admin()) return 0;
	creditNode* list = blacklistload();
	int cnt = show_all(list);
	int del = getint("��������Ҫɾ���ĺ�������Ϣ����ţ�����0���أ���", 0, cnt);
	if (!del) {
		blacklistsave(list);
		return 0;
	}
	blacklist b = get_blacklist(list, del);
	del_cnt(list, del);
	//��¼��Ϣ����
	{	
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -24; //������ɾ��
		strcpy_s(info->tel, 20, b.id);
		time(&info->time);
		logadd(list, info);
		;
		logsave(list);
	}
	blacklistsave(list);
	return 0;
}