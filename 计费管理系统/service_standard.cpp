#include "service_standard.h"
#include "data_standard.h"
#include "global.h"
#include "data_log.h"

struct standard* getnew() {
	struct standard* newstandard = (struct standard*)malloc(sizeof(struct standard));
	if (!newstandard) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		color();
		exit(17);
	}
	char* name = getstr_letnum("�������׼����0-16�ֽڣ�ֻ����Ӣ����ĸ�����֣���", 0, 16);
	double gap = getdouble("������������շѱ�׼����ʱ����", 0, 72000);
	int minute = getint("������ͳ�Ƹ��շѱ�׼���ڣ����ӣ���������", 1, 72000);
	double money = getdouble("������ÿ���ڻ��ѣ�", 0, 10000);
	double rate = money / minute;

	strcpy_s(newstandard->name, name);
	newstandard->gap = gap;
	newstandard->minute = minute;
	newstandard->money = money;
	newstandard->rate = rate;
	newstandard->del = 0;

	return newstandard;
}

int standard_new() {
	if (!check_admin()) return 0;
	standardNode* list = standardload();
	int cnt = show_all(list);
	struct standard* temp = getnew();
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -10; //�Ʒѱ�׼����
		info->gap = (int)(temp->gap * 60.0);
		info->duration = temp->minute * 60;
		info->amount = (int)(temp->money * 100.0);
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	insert(list, temp);
	standardsave(list);
	return 0;
}

int standard_ask() {
	standardNode* list = standardload();
	show_all(list);
	standardsave(list);
	return 0;
}

int standard_change() {
	if (!check_admin()) return 0;
	standardNode* list = standardload();
	int count = show_all(list);
	if (!count) {
		//printf("Ŀǰ�޼Ʒѱ�׼���޷��޸ģ�\n");
		standardsave(list);
		return 0;
	}
	int change = getint("������Ҫ�޸ĵı�׼��ţ�����0ֱ�ӷ��أ���",0,count);
	if (!change) {
		standardsave(list);
		return 0;
	}
	struct standard* temp = getnew();
	standard s = get_standard(list, change);
	change_stable(list, change, temp);
	standardsave(list);
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -12; //�Ʒѱ�׼�޸�
		info->duration = temp->minute * 60;
		info->amount = (int)(temp->money * 100.0);
		info->gap = (int)(temp->gap * 60.0);
		info->duration_pre = s.minute * 60;
		info->amount_pre = (int)(s.money * 100.0);
		info->gap_pre = (int)(s.gap * 60.0);
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	return 0;
}

int standard_delete() {
	if (!check_admin()) return 0;
	standardNode* list = standardload();
	int count = show_all(list);
	if (!count) {
		//printf("Ŀǰ�޼Ʒѱ�׼���޷�ɾ����\n");
		standardsave(list);
		return 0;
	}
	int del = getint("������Ҫɾ���ı�׼��ţ�����0ֱ�ӷ��أ���", 0, count);
	if (!del) {
		standardsave(list);
		return 0;
	}
	standard s = get_standard(list, del);
	del_cnt_saved(list, del);
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -14; //�Ʒѱ�׼ɾ��
		info->duration = s.minute * 60;
		info->amount = (int)(s.money * 100.0);
		info->gap = (int)(s.gap * 60.0);
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	standardsave(list);
	return 0;
}
