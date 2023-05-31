#include "global.h"
#include "data_vip.h"
#include "data_log.h"

int vip_new() {
	if (!check_admin()) return 0;
	vipNode* list = vipload();
	char* name = getstr("�������Ա����������6����/12��ĸ���֣���", 0, 12);
	int dis = getint("�������ۿ۰ٷֱ�����0-100����85��Ӧ��85�ۣ�90��Ӧ����ۣ���", 0, 100);
	int gap = getint("������ﵽ�ü����Ա����һ���Գ�ֵ��Ԫ����", 0, 99999);
	show_all_color();
	int color = getint("��������ֻ�Ա�ĸ�����ʾɫ�ţ�", 1, 15);
	struct vip* newvip = (struct vip*)calloc(1, sizeof(struct vip));
	if (!newvip) {

		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		::color();
		free(name);
		return -1;
	}
	newvip->discount = dis;
	strcpy_s(newvip->id, 16, name);
	newvip->gap = gap;
	newvip->color = color;
	insert(list, newvip);

	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -50; //��Ա�ȼ�����
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
	int change = getint("������Ҫ�޸ĵĻ�Ա��ţ�����0���أ���", 0, vipcnt);
	if (!change) {
		vipsave(list);
		return 0;
	}
	char* name = getstr("�������Ա����������6����/12��ĸ���֣���", 0, 12);
	int dis = getint("�������ۿ۰ٷֱ�����0-100����85��Ӧ��85�ۣ�90��Ӧ����ۣ���", 0, 100);
	int gap = getint("������ﵽ�ü����Ա����һ���Գ�ֵ��Ԫ����", 0, 99999);
	show_all_color();
	int color = getint("��������ֻ�Ա�ĸ�����ʾɫ�ţ�", 1, 15);
	struct vip* newvip = (struct vip*)calloc(1, sizeof(struct vip));
	if (!newvip) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
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
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -52; //��Ա�ȼ��޸�
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
	int del = getint("������Ҫɾ���Ļ�Ա��ţ�����0���أ���", 0, vipcnt);
	if (!del) {
		vipsave(list);
		return 0;
	}
	vip v = get_vip(list, del);
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -54; //��Ա�ȼ�ɾ��
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