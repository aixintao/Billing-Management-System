//һЩ���ÿ������ȫ�ֺ���
#include "global.h"
#include "data_credit.h"
#include <windows.h>

//�汾��
char version[] = "ver<1.0.0> by aixintao2022";

//�ⲿ�ļ���
char menufile[] = "menu.txt";
char submenufile[] = "submenu.txt";

char cardfile[] = "card.dat";
char standardfile[] = "standard.dat";
char vipfile[] = "vip.dat";
char blacklistfile[] = "blacklist.dat";
char adminfile[] = "admin.dat";
char logfile[] = "log.dat";

//��ֵ������ʱһ�γ�ֵ��������
int moneyleast = 1;
int moneymost = 20000;
//�����������̴���
int passwordlimit = 3;
//����Ƿ��ʱ�䣨�졤Ԫ��
//������Ϊ3000����Ƿ1Ԫ3000��/Ƿ100Ԫ30��ή��100�����ö�
//���öȹ�����õ绰���������������޷�������ʹ��
double debttime = 1;

//����Ա״̬���
int admin = 0;
//�ѵ�¼����Ա���˺�
char adminid[24] = "";

//������Ϣ��ɫ��
short color_err = 12;

//clear invaild inputs
void eatline() {
	while (getchar() != '\n');
}

//�������������(lower-upper)��
void input_msg(int lower, int upper) {
	printf("�������������(%d-%d)��", lower, upper);
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

//�����������ķ�װ
int getint(const char* msg, int lower, int upper) {
	printf(msg);
	int ret = getopt(lower, upper);
	return ret;
}

//�����븡�����ķ�װ
double getdouble(const char* msg, double lower, double upper) {
	printf(msg);
	double dbl = getlf(lower, upper);
	return dbl;
}

//����������ֺ���ĸ�ַ����ķ�װ
char* getstr_letnum(const char* msg, int lower,int upper) {
	if (upper < 0 || lower < 0 || upper > 1e7) return 0;
	char* temp = (char*)calloc(((long long)upper + 204) , sizeof(char));
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
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
				printf("�����ַ�����/���������������룺");
				flag = 1;
				eatline();
				continue;
			}
			for (int i = 0; i < ltemp; i++) {
				if (temp[i] >= 'A' && temp[i] <= 'Z' || temp[i] >= 'a' && temp[i] <= 'z' || temp[i] >= '0' && temp[i] <= '9');
				else {
					printf("�����ַ����Ϸ������������룺");
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
				printf("�����ַ����������������룺");
				flag = 1;
				eatline();
				continue;
			}
			for (int i = 0; i < ltemp; i++) {
				if (temp[i] >= 'A' && temp[i] <= 'Z' || temp[i] >= 'a' && temp[i] <= 'z' || temp[i] >= '0' && temp[i] <= '9');
				else {
					printf("�����ַ����Ϸ������������룺");
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

//�������ַ����ķ�װ
char* getstr(const char* msg, int lower, int upper) {
	if (upper < 0 || lower < 0 || upper > 1e7) return 0;
	char* temp = (char*)calloc((long long)upper + 204, sizeof(char));
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
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
				printf("�����ַ�����/���������������룺");
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
				printf("�����ַ����������������룺");
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

//������绰����ķ�װ
//����+�š�-�š�#�š��ո����֣��20λ�������������һ������
char* gettel(const char* msg) {
	char* temp = (char*)calloc((24), sizeof(char));
	const int upper = 20;
	int banned = 0;
	if (!temp) {
		colorerr();
		printf("�޷������ڴ棡�����ڴ�ʣ��ռ�\n");
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
			printf("�����ַ�����/���������������룺");
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
			printf("�����ַ����Ϸ������������룺");
			eatline();
			continue;
		}
		creditNode* blist = blacklistload();
		if (search_exist(blist, temp)) {
			printf("�õ绰�����ѱ���������������������룺");
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

//������Ǯ���ķ�װ������1�����²��֣����ط�Ǯ������ֵ��
//�����޽��ڡ�2,100,000֮��
int getcurrency(const char* msg, double lower, double upper) {
	if (lower > 2.1e7 || upper > 2.1e7) {
		colorerr();
		printf("%s������������!", __func__);
		color();
		return 114514514;
	}
	if (lower < -2.1e7 || upper < -2.1e7) {
		colorerr();
		printf("%s������������!", __func__);
		color();
		return 114514514;
	}
	double temp = 0;
	printf(msg);
	scanf_s("%lf", &temp);
	eatline();
	while (temp < lower || temp > upper) {
		printf("���벻���Ϸ�Χ��%.2lf-%.2lf�������������룺", lower, upper);
		scanf_s("%lf", &temp);
		eatline();
	}
	int money = int(temp * 100);
	return money;
}

void input_error() {
	printf("���벻��ȷ�����������룺");
}

void input_error(int left, int right) {
	printf("���벻���Ϸ�Χ(%d-%d)�����������룺", left, right);
}

void input_error(double left, double right) {
	printf("���벻���Ϸ�Χ(%.2lf-%.2lf)�����������룺", left, right);
}

FILE* openfp(FILE* fp, const char* filename, const char* mode) {
	fopen_s(&fp, filename, mode);
	if (fp == NULL) {
		colorerr();
		printf("�޷���%s����������Ƿ���:(\n", filename);
		color(); 
		exit(256);
	}
	return fp;
}

void TimeToString(time_t time, char* str) {
	struct tm* timeinfo = (struct tm*)malloc(sizeof(struct tm));
	if (!timeinfo) {
		colorerr();
		printf("TimeToString�������ִ���");;
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
		printf("TimeToString�������ִ���");;
		color();
		return 0;
	}
	time_t time1;
	sscanf_s(timestr, "%d-%d-%d %d:%d:%d", &tm1->tm_year, &tm1->tm_mon,
		&tm1->tm_mday, &tm1->tm_hour, &tm1->tm_min, &tm1->tm_sec);
	tm1->tm_year -= 1900;  // ���Ϊ��1900�꿪ʼ
	tm1->tm_mon -= 1;      // �·�Ϊ0~11
	tm1->tm_isdst = -1;
	time1 = mktime(tm1);
	free(tm1);
	return time1;
}

//��ȡ����
int getpsw(const char* st, char* buf) {
	int errs = 0;
	printf("���������룺");
	scanf_s("%[^\n]", buf, 18);
	eatline();
	while (strcmp(st, buf)) {
		errs++;
		if (errs >= passwordlimit) {
			colorerr();
			printf("�������������࣡ x_x\n");
			color();
			return -1;
		}
		printf("���벻��ȷ�����ٴ����루�Ѵ�%d�Σ��ﵽ%d�ν�ֱ�ӷ��أ���", errs, passwordlimit);
		scanf_s("%[^\n]", buf, 18);
		eatline();
	}
	return 0;
}

//�޸������ɫ��Ĭ��Ϊ����ɫ��
void color(int index) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), index);
}

//������Ϣ��ɫ
void colorerr() {
	if (color_err > 0 && color_err < 16) color(color_err);
	else color(12);
}

void show_all_color() {
	printf("ȫ����ɫ�б�");
	for (int i = 1; i <= 15; i++) {
		color(i);
		printf("%d�� ", i);
	}
	putchar('\n');
	color();
}

//������Ա�ĵ�¼״̬
int check_admin() {
	if (!admin) {
		colorerr();
		printf("��δ��¼����Ա����Ȩ�޲�����\n");
		color();
	}
	return admin;
}

//�ж����ںϷ��ԣ�1980��Ԫ��-2199��12��31�գ�
int judday(int yy, int mm, int dd) {
	//1980��Ϊ�ޣ���ֹ�߽����
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

//�����������գ���xxxx-xx-xx��ʽ��
time_t getday(const char* msg) {
	time_t res = 0;
	printf(msg);
	int yy, mm, dd;
	int flag = scanf_s("%d-%d-%d", &yy, &mm, &dd);
	//�Զ���Ƿ����Ӳ����ж�
	if (getchar() != '\n') {
		eatline();
		flag = 0;
	}
	if (!judday(yy, mm, dd)) flag = 0;
	while (flag != 3) {
		colorerr();
		printf("ʱ�������������������룺"); 
		color();
		flag = scanf_s("%d-%d-%d", &yy, &mm, &dd);
		//�Զ���Ƿ����Ӳ����ж�
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

//�����������յ���ʾ��Ϣ����xxxx-xx-xx��ʽ��
void getday_tips() {
	printf("���������ʽ����λ���-��-�գ��� 2023-2-22 �� 1997-12-31\n");
	printf("���ڽ��ܷ�Χ��1980-1-1 �� 2199-12-31\n");
}
