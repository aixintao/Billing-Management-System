#include "global.h"
#include "data_vip.h"
#include "data_log.h"

int vip_new() {
	if (!check_admin()) return 0;
	vipNode* list = vipload();
	char* name = getstr("请输入会员名（不超过6汉字/12字母数字）：", 0, 12);
	int dis = getint("请输入折扣百分比数（0-100，如85对应打85折，90对应打九折）：", 0, 100);
	int gap = getint("请输入达到该级别会员所需一次性充值金额（元）：", 0, 99999);
	show_all_color();
	int color = getint("请输入该种会员的高亮显示色号：", 1, 15);
	struct vip* newvip = (struct vip*)calloc(1, sizeof(struct vip));
	if (!newvip) {

		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		::color();
		free(name);
		return -1;
	}
	newvip->discount = dis;
	strcpy_s(newvip->id, 16, name);
	newvip->gap = gap;
	newvip->color = color;
	insert(list, newvip);

	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -50; //会员等级增加
		info->amount = dis;
		info->duration = gap*100;
		time(&info->time);
		strcpy_s(info->vip, 16, name);
		info->vipcolor = color;
		logadd(list, info);
		logsave(list);
	}
	vipsave(list);
	free(name);
	free(newvip);
	return 0;
}

int	vip_ask() {
	vipNode* list = vipload();
	show_all(list);
	vipsave(list);
	return 0;
}

int	vip_change() {
	if (!check_admin()) return 0;
	vipNode* list = vipload();
	int vipcnt = show_all(list);
	if (!vipcnt) {
		vipsave(list);
		return 0;
	}
	int change = getint("请输入要修改的会员序号（输入0返回）：", 0, vipcnt);
	if (!change) {
		vipsave(list);
		return 0;
	}
	char* name = getstr("请输入会员名（不超过6汉字/12字母数字）：", 0, 12);
	int dis = getint("请输入折扣百分比数（0-100，如85对应打85折，90对应打九折）：", 0, 100);
	int gap = getint("请输入达到该级别会员所需一次性充值金额（元）：", 0, 99999);
	show_all_color();
	int color = getint("请输入该种会员的高亮显示色号：", 1, 15);
	struct vip* newvip = (struct vip*)calloc(1, sizeof(struct vip));
	if (!newvip) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		::color();
		free(name);
		return -1;
	}
	newvip->discount = dis;
	strcpy_s(newvip->id, 16, name);
	newvip->gap = gap;
	newvip->color = color;
	vip v = get_vip(list, change);
	change_stable(list,change,newvip);
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -52; //会员等级修改
		info->amount = newvip->discount;
		info->duration = newvip->gap * 100;
		info->amount_pre = v.discount;
		info->duration_pre = v.gap * 100;
		time(&info->time);
		strcpy_s(info->vip_pre, 16, v.id);
		strcpy_s(info->vip, 16, newvip->id);
		info->vipcolor = v.color;
		info->vipcolor = newvip->color;
		logadd(list, info);
		logsave(list);
	}
	vipsave(list);
	free(newvip);
	free(name);
	return 0;
}

int	vip_delete() {
	if (!check_admin()) return 0;
	vipNode* list = vipload();
	int vipcnt = show_all(list);
	if (!vipcnt) {
		vipsave(list);
		return 0;
	}
	int del = getint("请输入要删除的会员序号（输入0返回）：", 0, vipcnt);
	if (!del) {
		vipsave(list);
		return 0;
	}
	vip v = get_vip(list, del);
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -54; //会员等级删除
		info->amount = v.discount;
		info->duration = v.gap * 100;
		time(&info->time);
		strcpy_s(info->vip, 16, v.id);
		info->vipcolor = v.color;
		logadd(list, info);
		logsave(list);
	}
	del_cnt(list, del);
	vipsave(list);
	return 0;
}