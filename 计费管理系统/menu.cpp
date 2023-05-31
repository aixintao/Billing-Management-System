#include "global.h"
#include "service_getonoff.h"
#include "service_vip.h"
#include "service_standard.h"
#include "service_fee.h"
#include "service_card.h"
#include "service_adminlogin.h"
#include "service_activity.h"
#include "service_askstats.h"
#include "service_credit.h"
#include "service_blacklist.h"
#include "menu.h"

extern char version[];
const int MAIN_LOWER = 0;
const int MAIN_UPPER = 9;
static int menu_time = 0;
FILE* fp;

void openfp() {
	fopen_s(&fp, "sub_menus.txt", "r");
	if (fp == NULL) {
		colorerr();
		printf("无法打开sub_menus.txt！请检查软件是否损坏:(\n");
		exit(256);
	}
}

int showmenu() {
	FILE* fp;
	int lineno = 0;
	fopen_s(&fp,"menu.txt", "r");
	if (fp == NULL) {
		colorerr();
		printf("无法打开menu.txt！请检查软件是否损坏:(\n");
		return -1;
	}
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("menu中无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	if (admin) {
		if (admin == 5) color(14);
		printf("当前已登录管理员账号：%s\n", adminid);
		color();
	}
	while (!feof(fp)) {
		fgets(buf,1000,fp);
		//show version
		if (lineno == 1) {
			buf[strlen(buf) - 1] = 0;
			printf("%s", buf);
			printf("%s\n", version);
			lineno++;
			continue;
		}
		printf("%s", buf);
		lineno++;
	}
	putchar('\n'); 
	input_msg(MAIN_LOWER,MAIN_UPPER);
	int opt = getopt(MAIN_LOWER, MAIN_UPPER);
	free(buf);
	fclose(fp);
	return opt;
}

int getonoff_menu() {
	int LOWER = 0,UPPER = -2;
	int cnt = 0;
	const int index = 1;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf,1000,fp);
	char c;
	while ((c=fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt){
	case 0:
		return 0;
	case 1:
		geton();
		break;
	case 2:
		getoff();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		color();
		return -1;
	}
	free(buf);
	return 0;
}

int fee_menu() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 2;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt) {
	case 0:
		return 0;
	case 1:
		recharge();
		break;
	case 2:
		refund();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		return -1;
	}
	free(buf);
	return 0;
}

int standard_menu() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 3;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt) {
	case 0:
		return 0;
	case 1:
		standard_new();
		break;
	case 2:
		standard_ask();
		break;
	case 3:
		standard_change();
		break;
	case 4:
		standard_delete();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		return -1;
	}
	free(buf);
	return 0;
}

int card_menu() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 4;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt) {
	case 0:
		return 0;
	case 1:
		card_new();
		break;
	case 2:
		card_ask();
		break;
	case 3:
		card_delete();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		return -1;
	}
	free(buf);
	return 0;
}

int black_menu() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 5;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt) {
	case 0:
		return 0;
	case 1:
		blacklist_ask();
		break;
	case 2:
		blacklist_delete();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		return -1;
	}
	free(buf);
	return 0;
}

int vip_menu() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 6;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt) {
	case 0:
		return 0;
	case 1:
		vip_new();
		break;
	case 2:
		vip_ask();
		break;
	case 3:
		vip_change();
		break;
	case 4:
		vip_delete();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		return -1;
	}
	free(buf);
	return 0;
}

int askstats_menu() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 7;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt) {
	case 0:
		return 0;
	case 1:
		stats_turnover();
		break;
	case 2:
		stats_cards();
		break;
	case 3:
		show_statements();
		break;
	case 4:
		show_logs();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		return -1;
	}
	free(buf);
	return 0;
}

int adminlogin_menu() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 8;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	input_msg(LOWER, UPPER);
	int opt = getopt(LOWER, UPPER);
	switch (opt) {
	case 0:
		return 0;
	case 1:
		admin_login();
		break;
	case 2:
		change_password();
		break;
	case 3:
		admin_new();
		break;
	case 4:
		admin_logout();
		break;
	case 5:
		admin_delete();
		break;
	default:
		colorerr();
		printf("程序错误或sub_menus.txt文件有误！\n");
		return -1;
	}
	free(buf);
	return 0;
}

int credits() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 9;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	free(buf);
	return 0;
}

int bye() {
	int LOWER = 0, UPPER = -2;
	int cnt = 0;
	const int index = 10;
	char* buf = (char*)malloc(1000 * sizeof(char));
	if (!buf) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		fclose(fp);
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	while (cnt < index) {
		if (fgetc(fp) == '&') cnt++;
	}
	fgets(buf, 1000, fp);
	char c;
	while ((c = fgetc(fp)) != '&') {
		putchar(c);
		if (c == '\n') UPPER++;
	}
	free(buf);
	save_log_sys(-999);
	return 0;
}

int menu() {
	//color(14);
	if (!menu_time) save_log_sys(-998);
	menu_time++;
	menu_load();
	openfp();
	int opt = showmenu();
	switch (opt) {
	case 1:
		getonoff_menu();
		break;
	case 2:
		fee_menu();
		break;
	case 3:
		standard_menu();
		break;
	case 4:
		card_menu();
		break;
	case 5:
		black_menu();
		break;
	case 6:
		vip_menu();
		break;
	case 7:
		askstats_menu();
		break;
	case 8:
		adminlogin_menu();
		break;
	case 9:
		credits();
		break;
	case 0:
		bye();
		break;
	}
	fclose(fp);
	return opt;
}
