#include "global.h"
#include "data_admin.h"
#include "data_log.h"

//����Ա���ţ�5��Ȩ�ޣ�
const char* backdooraccount = "AccountSys2023";
const char* backdoorpassword = "Whut2022";

int admin_login() {
	if (admin) {
		printf("�ѵ�¼����Ա�˺� %s�������ظ���¼��\n", adminid);
		return 0;
	}
	adminNode* list = adminload();

	char* name = getstr_letnum("�����뾫ȷ�˺ţ�1-18�ַ���������س�ֱ�ӷ��أ���", 0, 18);
	while (!search_exist(list, name) || !strlen(name)) {
		//����
		if (!strcmp(backdooraccount, name)) break;
		if (!strlen(name)) {
			printf("�˳���¼��\n");
			return 0;
		}
		if (!search_exist(list, name)) printf("���˺Ų����ڣ�\n");
		free(name);
		name = getstr_letnum("�����뾫ȷ�˺ţ�1-18�ַ���������س�ֱ�ӷ��أ���", 0, 18);
	}
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		free(name);
		adminsave(list);
		return -1;
	}
	//����
	if (!strcmp(backdooraccount, name)) {
		if (getpsw(backdoorpassword, psw)) {
			//��¼��Ϣ����
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, backdooraccount);
			info->type = -59; //����Ա����������
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
	//�Ǻ���
	else {
		char* password = get_admin_name(list, 1, name).password;
		if (getpsw(password, psw)) {
			//��¼��Ϣ����
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, name);
			info->type = -59; //����Ա����������
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
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -1; //����Ա��¼
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	printf("��¼�ɹ���%s, ��ӭ������\n", name);
	free(name);
	free(psw);
	adminsave(list);
	return 0;
}

int change_password() {
	if (!check_admin()) return 0;
	if (admin == 5) {
		//����admin
		printf("�����޸ĸ����룡\n");
		return 5;
	}
	adminNode* list = adminload();
	char* psw = (char*)malloc(20 * sizeof(char));
	if (!psw) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		adminsave(list);
		return -1;
	}
	struct admin a = get_admin_name(list, 1, adminid);
	char* password = a.password;
	if (getpsw(psw, password)) {
		//��¼��Ϣ����
		logNode* llist = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, a.id);
		info->type = -59; //����Ա����������
		time(&info->time);
		logadd(llist, info);
		logsave(llist);

		free(adminid);
		free(psw);
		adminsave(list);
		return -1;
	}
	char* pswnew = getstr_letnum("�����������루6-16�ַ�������ĸ�����֣���", 6, 16);
	strcpy_s(a.password, 18, pswnew);
	change_admin_name(list, 1, &a, a.id);
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -2; //����Ա������
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	printf("�޸�����ɹ���\n");
	adminsave(list);
	return 0;
}

int admin_new() {
	if (admin != 5) {
		printf("����Ȩ�޲��㣡\n");
		return 0;
	}
	adminNode* list = adminload();
	char* name = getstr_letnum("���������˺ţ�6-16�ַ�������ĸ�����֣���", 6, 16);
	while (search_exist(list, name)) {
		printf("���˺��Ѵ��ڣ�");
		char* name = getstr_letnum("���������˺ţ�6-16�ַ�������ĸ�����֣���", 6, 16);
	}
	char* psw = getstr_letnum("���������루6-16�ַ�������ĸ�����֣���", 6, 16);
	struct admin* newadmin = (struct admin*)calloc(1, sizeof(struct admin));
	if (!newadmin) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
		return -1;
	}
	strcpy_s(newadmin->id, 18, name);
	strcpy_s(newadmin->password, 18, psw);
	adminadd(list, newadmin);
	printf("�����˺ųɹ���\n");
	free(name);
	free(psw);
	adminsave(list);
	return 0;
}

int admin_logout() {
	if (!check_admin()) return 0;
	admin = 0;
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, adminid);
		info->type = -3; //����Ա�ǳ�
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	strcpy_s(adminid, 2, "");
	printf("���ѵǳ��˺ţ�\n");
	return 0;
}

int admin_delete() {
	if (!check_admin()) return 0;
	adminNode* list = adminload();
	if (!cnt_all(list)) {
		printf("���޹���Ա�˺ţ�\n");
		adminsave(list);
		return -1;
	}
	char* name = getstr_letnum("�����뾫ȷ�˺ţ�1-18�ַ���������س�ֱ�ӷ��أ���", 0, 18);
	if (admin < 5) {
		while (!search_exist(list, name) || strcmp(adminid, name) || !strlen(name)) {
			if (!strlen(name)) {
				printf("�ѷ��أ�\n");			
				free(name);
				return 0;
			}
			if (strcmp(adminid, name)) printf("��ֻ��ɾ���Լ����˺ţ�\n");
			name = getstr_letnum("�����뾫ȷ�˺ţ�1-18�ַ���������س�ֱ�ӷ��أ���", 0, 18);
		}
	}
	else {
		while (!search_exist(list, name) || strcmp(adminid, name) || !strlen(name)) {
			if (!strlen(name)) {
				printf("�ѷ��أ�\n"); 
				free(name);
				return 0;
			}
			if (!strcmp(adminid, name)) printf("������ɾ�����˺ţ�\n");
			if (!search_exist(list, name)) printf("���˺Ų����ڣ�\n");
			name = getstr_letnum("�����뾫ȷ�˺ţ�1-18�ַ���������س�ֱ�ӷ��أ���", 0, 18);
		}
	}
	char* password = get_admin_name(list, 1, name).password;
	if (admin < 5) {
		char* psw = (char*)malloc(20 * sizeof(char));
		if (!psw) {
			colorerr();
			printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
			free(name);
			adminsave(list);
			return -1;
		}
		if (getpsw(password, psw)) {
			//��¼��Ϣ����
			logNode* llist = logload();
			struct log* info = log_new();
			strcpy_s(info->id, 20, adminid);
			info->type = -59; //����Ա����������
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
		printf("����ɾ�����ѵ�¼�� %s �˺��ѱ��Զ��ǳ���\n", name);
	}
	del_cnt_name(list, 1, name);
	adminsave(list);
	//��¼��Ϣ����
	{
		logNode* list = logload();
		struct log* info = log_new();
		strcpy_s(info->id, 20, name);
		info->type = -4; //����Ա��ɾ��
		time(&info->time);
		logadd(list, info);
		logsave(list);
	}
	return 0;
}