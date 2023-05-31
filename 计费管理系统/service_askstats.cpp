#include "global.h"
#include "data_card.h"
#include "data_log.h"

//��������/�ر���Ϣ
int save_log_sys(int id) {
	logNode* list = logload();
	struct log* info = log_new();
	info->type = id; //����/�˳�
	time(&info->time);
	logadd(list, info);
	logsave(list);
	return 0;
}

//ͳ��ָ��ʱ��Ӫҵ��ܳ�ֵ���������Ѷ
int stats_turnover() {
	if (!check_admin()) return 0;
	getday_tips();
	time_t set = getday("�������ѯ��ʼ���ڣ������գ���");
	time_t end = getday("�������ѯ��ֹ���ڣ������գ���") + 86399ll; //����һ��ȫ��������
	while (end < set) {
		printf("��ֹ��������ʼ����֮ǰ�����������룡\n");
		end = getday("�������ѯ��ֹ���ڣ������գ���") + 86399ll; //����һ��ȫ��������
	}
	char s1[88] = "\0";
	char s2[88] = "\0";
	TimeToString(set, s1);
	TimeToString(end, s2);
	printf("ʱ�䷶Χ��%s �� %s\n", s1, s2);
	long long sumbalance = 0;

	logNode* list = logload();
	sumbalance = cnt_all_time_charge(list, set, end);
	printf("�ܳ�ֵ��%.2lfԪ\n", sumbalance / 100.0);

	long long refundbalance = cnt_all_time_refund(list, set, end);
	printf("���˷ѽ�%.2lfԪ\n", refundbalance / 100.0);
	double netturnover = (sumbalance - refundbalance) / 100.0;
	printf("�������%.2lfԪ\n", netturnover);

	sumbalance = cnt_all_time_getoff(list, set, end);
	printf("���»����Ѷ%.2lfԪ\n", sumbalance / 100.0);
	logsave(list);
	return 0;
}

//ͳ�ƿ������ܿ����
int stats_cards() {
	if (!check_admin()) return 0;
	CardNode* list = cardload();
	int cnt = cnt_all(list);
	int cnt_total = cnt_all_saved(list);
	long long sumbalance = get_sum_balance(list);
	printf("����Ч������%d\n", cnt);
	printf("��ע��������%d\n", cnt_total - cnt);
	printf("�ܿ���������ע��������%d\n", cnt_total);
	printf("�ܿ���%.2lfԪ\n", sumbalance / 100.0);
	cardsave(list);
	return 0;
}

//ͳ��ָ��ʱ���ڳ�ֵ�˷ѽ��
int show_statements() {
	if (!check_admin()) return 0;
	getday_tips();
	time_t set = getday("�������ѯ��ʼ���ڣ������գ���");
	time_t end = getday("�������ѯ��ֹ���ڣ������գ���") + 86399ll; //����һ��ȫ��������
	while (end < set) {
		printf("��ֹ��������ʼ����֮ǰ�����������룡\n");
		end = getday("�������ѯ��ֹ���ڣ������գ���") + 86399ll; //����һ��ȫ��������
	}
	char s1[88] = "\0";
	char s2[88] = "\0";
	TimeToString(set, s1);
	TimeToString(end, s2);
	printf("\nʱ�䷶Χ��%s �� %s\n\n", s1, s2);

	{
		logNode* list = logload();
		printf("\nʱ�䷶Χ���ܳ�ֵ�%.2lfԪ\n", cnt_all_time_charge(list, set, end) / 100.0);
		int cnt = show_all_time_charge(list, set, end);
		printf("\nʱ�䷶Χ�����˷Ѷ%.2lfԪ\n", cnt_all_time_refund(list, set, end) / 100.0);
		cnt = show_all_time_refund(list, set, end);
		logsave(list);
	}
	putchar('\n');
	return 0;
}

//ͳ��ָ��ʱ�����¼���־
int show_logs() {
	if (!check_admin()) return 0;
	getday_tips();
	time_t set = getday("�������ѯ��ʼ���ڣ������գ���");
	time_t end = getday("�������ѯ��ֹ���ڣ������գ���") + 86399ll; //����һ��ȫ��������
	while (end < set) {
		printf("��ֹ��������ʼ����֮ǰ�����������룡\n");
		end = getday("�������ѯ��ֹ���ڣ������գ���") + 86399ll; //����һ��ȫ��������
	}
	char s1[88] = "\0";
	char s2[88] = "\0";
	TimeToString(set, s1);
	TimeToString(end, s2);
	if (admin >= 5) {
		char* name = getstr_letnum("�������ѯ����/����Ա�˺ţ�0-16�ַ���ֻ����Ӣ����ĸ�����֣�ֱ�ӻس����ȫ������", 0, 16);
		printf("ʱ�䷶Χ��%s �� %s\n\n", s1, s2);
		logNode* list = logload();
		show_all_name_time(list, name, set, end);
		logsave(list);
	}
	else {
		char* name = getstr_letnum("�������ѯ���ţ�0-16�ַ���ֻ����Ӣ����ĸ�����֣�ֱ�ӻس����ȫ������", 0, 16);
		printf("ʱ�䷶Χ��%s �� %s\n\n", s1, s2);
		logNode* list = logload();
		show_all_cardname_time(list, name, set, end);
		logsave(list);
	}
	return 0;
}

