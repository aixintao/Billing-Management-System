#include "service_standard.h"
#include "data_standard.h"
#include "global.h"
#include "data_log.h"

struct standard* getnew() {
	struct standard* newstandard = (struct standard*)malloc(sizeof(struct standard));
	if (!newstandard) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		exit(17);
	}
	char* name = getstr_letnum("请输入标准名（0-16字节，只允许英文字母和数字）：", 0, 16);
	double gap = getdouble("请输入满足该收费标准所需时长：", 0, 72000);
	int minute = getint("请输入统计该收费标准周期（分钟，整数）：", 1, 72000);
	double money = getdouble("请输入每周期花费：", 0, 10000);
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
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -10; //计费标准新增
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
		//printf("目前无计费标准，无法修改！\n");
		standardsave(list);
		return 0;
	}
	int change = getint("请输入要修改的标准编号（输入0直接返回）：",0,count);
	if (!change) {
		standardsave(list);
		return 0;
	}
	struct standard* temp = getnew();
	standard s = get_standard(list, change);
	change_stable(list, change, temp);
	standardsave(list);
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -12; //计费标准修改
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
		//printf("目前无计费标准，无法删除！\n");
		standardsave(list);
		return 0;
	}
	int del = getint("请输入要删除的标准编号（输入0直接返回）：", 0, count);
	if (!del) {
		standardsave(list);
		return 0;
	}
	standard s = get_standard(list, del);
	del_cnt_saved(list, del);
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -14; //计费标准删除
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
