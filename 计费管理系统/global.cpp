//一些公用库和自制全局函数
#include "global.h"
#include "data_credit.h"
#include <windows.h>

//版本号
char version[] = "ver<1.0.0> by aixintao2022";

//外部文件名
char menufile[] = "menu.txt";
char submenufile[] = "submenu.txt";

char cardfile[] = "card.dat";
char standardfile[] = "standard.dat";
char vipfile[] = "vip.dat";
char blacklistfile[] = "blacklist.dat";
char adminfile[] = "admin.dat";
char logfile[] = "log.dat";

//充值、开卡时一次充值的上下限
int moneyleast = 1;
int moneymost = 20000;
//密码错误的容忍次数
int passwordlimit = 3;
//允许欠费时间（天·元）
//如设置为3000，则欠1元3000天/欠100元30天会降低100点信用度
//信用度归零则该电话号码进入黑名单，无法开卡和使用
double debttime = 1;

//管理员状态标记
int admin = 0;
//已登录管理员的账号
char adminid[24] = "";

//错误信息颜色号
short color_err = 12;

//clear invaild inputs
void eatline() {
	while (getchar() != '\n');
}

//请输入操作代号(lower-upper)：
void input_msg(int lower, int upper) {
	printf("请输入操作代号(%d-%d)：", lower, upper);
}

//scanf_s("%d", &opt); with bounds
int getopt(int lower, int upper) {
	int opt = -1;
	scanf_s("%d", &opt);
	eatline();
	while (opt<lower || opt > upper) {
		input_error(lower, upper);
		scanf_s("%d", &opt);
		eatline();
	}
	return opt;
}

//scanf_s("%lf", &x); with bounds
double getlf(double lower, double upper) {
	double x = -1;
	scanf_s("%lf", &x);
	eatline();
	while (x<lower || x > upper) {
		input_error(lower, upper);
		scanf_s("%lf", &x);
		eatline();
	}
	return x;
}

//请输入整数的封装
int getint(const char* msg, int lower, int upper) {
	printf(msg);
	int ret = getopt(lower, upper);
	return ret;
}

//请输入浮点数的封装
double getdouble(const char* msg, double lower, double upper) {
	printf(msg);
	double dbl = getlf(lower, upper);
	return dbl;
}

//请输入仅数字和字母字符串的封装
char* getstr_letnum(const char* msg, int lower,int upper) {
	if (upper < 0 || lower < 0 || upper > 1e7) return 0;
	char* temp = (char*)calloc(((long long)upper + 204) , sizeof(char));
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return 0;
	}
	printf(msg);
	if (lower) {
		do {
			int flag = 0;
			//fgets(name, 18, stdin);
			//putc('\n', stdin);
			strcpy_s(temp, 5, "");
			scanf_s("%[^\n]", temp, upper + 200);
			size_t ltemp = strlen(temp);
			if (ltemp < lower || ltemp > upper) {
				printf("输入字符过短/过长！请重新输入：");
				flag = 1;
				eatline();
				continue;
			}
			for (int i = 0; i < ltemp; i++) {
				if (temp[i] >= 'A' && temp[i] <= 'Z' || temp[i] >= 'a' && temp[i] <= 'z' || temp[i] >= '0' && temp[i] <= '9');
				else {
					printf("输入字符不合法！请重新输入：");
					flag = 1;
					eatline();
					break;
				}
			}
			if (flag) continue;
			break;
		} while (1);
	}
	else {
		do {
			int flag = 0;
			strcpy_s(temp, 5, "");
			scanf_s("%[^\n]", temp, upper + 200);
			size_t ltemp = strlen(temp);
			if (ltemp > upper) {
				printf("输入字符过长！请重新输入：");
				flag = 1;
				eatline();
				continue;
			}
			for (int i = 0; i < ltemp; i++) {
				if (temp[i] >= 'A' && temp[i] <= 'Z' || temp[i] >= 'a' && temp[i] <= 'z' || temp[i] >= '0' && temp[i] <= '9');
				else {
					printf("输入字符不合法！请重新输入：");
					flag = 1;
					eatline();
					break;
				}
			}
			if (flag) continue;
			break;
		} while (1);
	}
	eatline();
	return temp;
}

//请输入字符串的封装
char* getstr(const char* msg, int lower, int upper) {
	if (upper < 0 || lower < 0 || upper > 1e7) return 0;
	char* temp = (char*)calloc((long long)upper + 204, sizeof(char));
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return 0;
	}
	printf(msg);
	if (lower) {
		do {
			int flag = 0;
			//fgets(name, 18, stdin);
			//putc('\n', stdin);

			strcpy_s(temp, 5, "");
			scanf_s("%[^\n]", temp, upper + 200);
			size_t ltemp = strlen(temp);
			if (ltemp < lower || ltemp > upper) {
				printf("输入字符过短/过长！请重新输入：");
				flag = 1;
				eatline();
				continue;
			}
			if (flag) {
				eatline();
				continue;
			}
			break;
		} while (1);
	}
	else {
		do {
			int flag = 0;
			strcpy_s(temp, 5, "");
			scanf_s("%[^\n]", temp, upper + 200);
			size_t ltemp = strlen(temp);
			if (ltemp > upper) {
				printf("输入字符过长！请重新输入：");
				flag = 1;
				eatline();
				continue;
			}
			if (flag) {
				eatline(); 
				continue;
			}
			break;
		} while (1);
	}
	eatline();
	return temp;
}

//请输入电话号码的封装
//允许+号、-号、#号、空格、数字，最长20位，必须包含至少一个数字
char* gettel(const char* msg) {
	char* temp = (char*)calloc((24), sizeof(char));
	const int upper = 20;
	int banned = 0;
	if (!temp) {
		colorerr();
		printf("无法分配内存！请检查内存剩余空间\n");
		color();
		return 0;
	}
	printf(msg);
	do {
		int flag = 0;
		//fgets(name, 18, stdin);
		//putc('\n', stdin);

		scanf_s("%[^\n]", temp, upper + 2);
		size_t ltemp = strlen(temp);
		if (ltemp <= 0 || ltemp > upper) {
			printf("输入字符过短/过长！请重新输入：");
			flag = 1;
			eatline();
			continue;
		}
		int cnt = 0;
		for (int i = 0; i < ltemp; i++) {
			if (temp[i] >= '0' && temp[i] <= '9') cnt++;
			else if (temp[i] == '-' || temp[i] == '+' || temp[i] == ' ' || temp[i] == '#');
			else {
				flag = 1;
				eatline();
				break;
			}
		}
		if (flag || !cnt) {
			printf("输入字符不合法！请重新输入：");
			eatline();
			continue;
		}
		creditNode* blist = blacklistload();
		if (search_exist(blist, temp)) {
			printf("该电话号码已被拉入黑名单！请重新输入：");
			blacklistsave(blist);
			eatline();
			continue;
		}
		blacklistsave(blist);
		break;
	} while (1);
	eatline();
	return temp;
}

//请输入钱数的封装（忽略1分以下部分，返回分钱的整数值）
//上下限介于±2,100,000之间
int getcurrency(const char* msg, double lower, double upper) {
	if (lower > 2.1e7 || upper > 2.1e7) {
		colorerr();
		printf("%s函数调用有误!", __func__);
		color();
		return 114514514;
	}
	if (lower < -2.1e7 || upper < -2.1e7) {
		colorerr();
		printf("%s函数调用有误!", __func__);
		color();
		return 114514514;
	}
	double temp = 0;
	printf(msg);
	scanf_s("%lf", &temp);
	eatline();
	while (temp < lower || temp > upper) {
		printf("输入不符合范围（%.2lf-%.2lf），请重新输入：", lower, upper);
		scanf_s("%lf", &temp);
		eatline();
	}
	int money = int(temp * 100);
	return money;
}

void input_error() {
	printf("输入不正确，请重新输入：");
}

void input_error(int left, int right) {
	printf("输入不符合范围(%d-%d)，请重新输入：", left, right);
}

void input_error(double left, double right) {
	printf("输入不符合范围(%.2lf-%.2lf)，请重新输入：", left, right);
}

FILE* openfp(FILE* fp, const char* filename, const char* mode) {
	fopen_s(&fp, filename, mode);
	if (fp == NULL) {
		colorerr();
		printf("无法打开%s！请检查软件是否损坏:(\n", filename);
		color(); 
		exit(256);
	}
	return fp;
}

void TimeToString(time_t time, char* str) {
	struct tm* timeinfo = (struct tm*)malloc(sizeof(struct tm));
	if (!timeinfo) {
		colorerr();
		printf("TimeToString函数出现错误！");;
		color();
		str = NULL;
		return;
	}
	localtime_s(timeinfo,&time);
	strftime(str, 35, "%Y-%m-%d %H:%M:%S", timeinfo);
}

time_t StringToTime(char* timestr) {
	struct tm* tm1 = (struct tm*)malloc(sizeof(struct tm));
	if (!tm1) {
		colorerr();
		printf("TimeToString函数出现错误！");;
		color();
		return 0;
	}
	time_t time1;
	sscanf_s(timestr, "%d-%d-%d %d:%d:%d", &tm1->tm_year, &tm1->tm_mon,
		&tm1->tm_mday, &tm1->tm_hour, &tm1->tm_min, &tm1->tm_sec);
	tm1->tm_year -= 1900;  // 年份为从1900年开始
	tm1->tm_mon -= 1;      // 月份为0~11
	tm1->tm_isdst = -1;
	time1 = mktime(tm1);
	free(tm1);
	return time1;
}

//获取密码
int getpsw(const char* st, char* buf) {
	int errs = 0;
	printf("请输入密码：");
	scanf_s("%[^\n]", buf, 18);
	eatline();
	while (strcmp(st, buf)) {
		errs++;
		if (errs >= passwordlimit) {
			colorerr();
			printf("密码错误次数过多！ x_x\n");
			color();
			return -1;
		}
		printf("密码不正确，请再次输入（已错%d次，达到%d次将直接返回）：", errs, passwordlimit);
		scanf_s("%[^\n]", buf, 18);
		eatline();
	}
	return 0;
}

//修改输出颜色（默认为暗白色）
void color(int index) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), index);
}

//错误信息颜色
void colorerr() {
	if (color_err > 0 && color_err < 16) color(color_err);
	else color(12);
}

void show_all_color() {
	printf("全部颜色列表：");
	for (int i = 1; i <= 15; i++) {
		color(i);
		printf("%d号 ", i);
	}
	putchar('\n');
	color();
}

//检查管理员的登录状态
int check_admin() {
	if (!admin) {
		colorerr();
		printf("您未登录管理员，无权限操作！\n");
		color();
	}
	return admin;
}

//判断日期合法性（1980年元旦-2199年12月31日）
int judday(int yy, int mm, int dd) {
	//1980年为限，防止边界出错
	if (yy < 1980 || yy > 2199) return 0;
	if (mm < 1 || mm >12) return 0;
	if (dd < 1) return 0;
	switch (mm) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if (dd > 31) return 0;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if (dd > 30) return 0;
		break;
	case 2:
		if (yy % 4 == 0 && yy % 100 || yy % 400 == 0) {
			if (dd > 29) return 0;
		} else {
			if (dd > 28) return 0;
		}
		break;
	default:
		return 0;
	}
	return 1;
}

//请输入年月日（以xxxx-xx-xx格式）
time_t getday(const char* msg) {
	time_t res = 0;
	printf(msg);
	int yy, mm, dd;
	int flag = scanf_s("%d-%d-%d", &yy, &mm, &dd);
	//对额外非法附加部分判断
	if (getchar() != '\n') {
		eatline();
		flag = 0;
	}
	if (!judday(yy, mm, dd)) flag = 0;
	while (flag != 3) {
		colorerr();
		printf("时间输入有误！请重新输入："); 
		color();
		flag = scanf_s("%d-%d-%d", &yy, &mm, &dd);
		//对额外非法附加部分判断
		if (getchar() != '\n') {
			eatline();
			flag = 0;
		}
		if (!judday(yy, mm, dd)) flag = 0;
	}
	char temp[32];
	sprintf_s(temp, 24, "%d-%d-%d 00:00:00", yy, mm, dd);
	res = StringToTime(temp);
	return res;
}

//请输入年月日的提示信息（以xxxx-xx-xx格式）
void getday_tips() {
	printf("日期输入格式：四位年份-月-日，如 2023-2-22 或 1997-12-31\n");
	printf("日期接受范围：1980-1-1 至 2199-12-31\n");
}
