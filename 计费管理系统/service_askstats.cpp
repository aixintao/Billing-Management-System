#include "global.h"
#include "data_card.h"
#include "data_log.h"

//保存启动/关闭信息
int save_log_sys(int id) {
	logNode* list = logload();
	struct log* info = log_new();
	info->type = id; //启动/退出
	time(&info->time);
	logadd(list, info);
	logsave(list);
	return 0;
}

//统计指定时间营业额（总充值数、总消费额）
int stats_turnover() {
	if (!check_admin()) return 0;
	getday_tips();
	time_t set = getday("请输入查询起始日期（含该日）：");
	time_t end = getday("请输入查询截止日期（含该日）：") + 86399ll; //把这一天全包括在内
	while (end < set) {
		printf("截止日期在起始日期之前！请重新输入！\n");
		end = getday("请输入查询截止日期（含该日）：") + 86399ll; //把这一天全包括在内
	}
	char s1[88] = "\0";
	char s2[88] = "\0";
	TimeToString(set, s1);
	TimeToString(end, s2);
	printf("时间范围：%s 至 %s\n", s1, s2);
	long long sumbalance = 0;

	logNode* list = logload();
	sumbalance = cnt_all_time_charge(list, set, end);
	printf("总充值金额：%.2lf元\n", sumbalance / 100.0);

	long long refundbalance = cnt_all_time_refund(list, set, end);
	printf("总退费金额：%.2lf元\n", refundbalance / 100.0);
	double netturnover = (sumbalance - refundbalance) / 100.0;
	printf("净收入金额：%.2lf元\n", netturnover);

	sumbalance = cnt_all_time_getoff(list, set, end);
	printf("总下机消费额：%.2lf元\n", sumbalance / 100.0);
	logsave(list);
	return 0;
}

//统计卡数、总卡余额
int stats_cards() {
	if (!check_admin()) return 0;
	CardNode* list = cardload();
	int cnt = cnt_all(list);
	int cnt_total = cnt_all_saved(list);
	long long sumbalance = get_sum_balance(list);
	printf("总有效卡数：%d\n", cnt);
	printf("总注销卡数：%d\n", cnt_total - cnt);
	printf("总卡数（包括注销卡）：%d\n", cnt_total);
	printf("总卡余额：%.2lf元\n", sumbalance / 100.0);
	cardsave(list);
	return 0;
}

//统计指定时间内充值退费金额
int show_statements() {
	if (!check_admin()) return 0;
	getday_tips();
	time_t set = getday("请输入查询起始日期（含该日）：");
	time_t end = getday("请输入查询截止日期（含该日）：") + 86399ll; //把这一天全包括在内
	while (end < set) {
		printf("截止日期在起始日期之前！请重新输入！\n");
		end = getday("请输入查询截止日期（含该日）：") + 86399ll; //把这一天全包括在内
	}
	char s1[88] = "\0";
	char s2[88] = "\0";
	TimeToString(set, s1);
	TimeToString(end, s2);
	printf("\n时间范围：%s 至 %s\n\n", s1, s2);

	{
		logNode* list = logload();
		printf("\n时间范围内总充值额：%.2lf元\n", cnt_all_time_charge(list, set, end) / 100.0);
		int cnt = show_all_time_charge(list, set, end);
		printf("\n时间范围内总退费额：%.2lf元\n", cnt_all_time_refund(list, set, end) / 100.0);
		cnt = show_all_time_refund(list, set, end);
		logsave(list);
	}
	putchar('\n');
	return 0;
}

//统计指定时间内事件日志
int show_logs() {
	if (!check_admin()) return 0;
	getday_tips();
	time_t set = getday("请输入查询起始日期（含该日）：");
	time_t end = getday("请输入查询截止日期（含该日）：") + 86399ll; //把这一天全包括在内
	while (end < set) {
		printf("截止日期在起始日期之前！请重新输入！\n");
		end = getday("请输入查询截止日期（含该日）：") + 86399ll; //把这一天全包括在内
	}
	char s1[88] = "\0";
	char s2[88] = "\0";
	TimeToString(set, s1);
	TimeToString(end, s2);
	if (admin >= 5) {
		char* name = getstr_letnum("请输入查询卡号/管理员账号（0-16字符，只允许英文字母和数字；直接回车输出全部）：", 0, 16);
		printf("时间范围：%s 至 %s\n\n", s1, s2);
		logNode* list = logload();
		show_all_name_time(list, name, set, end);
		logsave(list);
	}
	else {
		char* name = getstr_letnum("请输入查询卡号（0-16字符，只允许英文字母和数字；直接回车输出全部）：", 0, 16);
		printf("时间范围：%s 至 %s\n\n", s1, s2);
		logNode* list = logload();
		show_all_cardname_time(list, name, set, end);
		logsave(list);
	}
	return 0;
}

