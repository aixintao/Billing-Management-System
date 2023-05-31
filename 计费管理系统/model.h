#pragma once
#include <time.h>
//���ṹ��
struct Card {
	time_t set;  //����ʱ��
	time_t geton;    //�ϻ�ʱ��
	time_t getoff;   //�»�ʱ��
	char id[20]; //����
	char password[20];//����	
	char tel[24]; //�绰
	int balance; //�������ʾ���ٷ�Ǯ����ֹ�����������
	int credit; //����ֵ
	int vip; //��Ա�ȼ�
	int on; //�Ƿ����ϻ���
	int usecount; //ʹ�ô���
	int del; //ɾ����־
};

//�Ʒѱ�׼���ֵ��Ʒ��� v0.2.1����
struct standard {
	char name[18]; //����
	double gap; //�����׼��������������ʱ��
	int minute; //ÿ��������
	double money; //����Ԫ
	double rate; //ÿ���ӻ��ѣ��Զ����㣩
	int del; //ɾ�����
};

//ͨ���¼���־
struct log {
	char id[20]; //��ؿ���/����
	int type; //�¼�����
	// 0
	// -1 ����Ա��¼
	// -2 ����Ա������
	// -3 ����Ա�ǳ�
	// -4 ����Ա��ɾ��
	// -10 �Ʒѱ�׼����
	// -12 �Ʒѱ�׼�޸�
	// -14 �Ʒѱ�׼ɾ��
	// -20 ���õ����������
	// -21 �ֻ���ʧ�ſ�ע��
	// -24 ������ɾ��
	// -50 ��Ա�ȼ�����
	// -52 ��Ա�ȼ��޸�
	// -54 ��Ա�ȼ�ɾ��
	// -59 ����Ա����������
	// -998 ϵͳ����
	// -999 ϵͳ�ر�
	// 1 �ϻ�
	// 2 �»�
	// 3 ��ֵ
	// 4 �˷�
	// 10 ��ӿ�
	// 12 ������
	// 14 ɾ����
	// 20 ������������
	// 30 ������Ա
	int amount; //��ؽ��
	int balance;//������
	int gap; //�����������
	int gap_pre; //���ԭ��������
	int duration; //����ϻ�ʱ��
	int duration_pre; //���ԭ�ϻ�ʱ��
	int amount_pre; //���ԭ���
	time_t time;//���ʱ��
	char tel[24]; //����ֻ���
	char vip[16];//��ػ�Աͷ��
	char vipcolor; //��ػ�Ա��ɫ
	char vip_pre[16];//���ԭ��Աͷ��
	char vipcolor_pre; //���ԭ��Ա��ɫ
	int del; //ɾ����־
};

//��Ա�ȼ�
struct vip {
	char id[16]; //��Ա�ȼ���
	int gap; //һ���Գ�ֵ����λ�������ܸ�vip
	int color; //��Ա������ʾɫ
	int discount; //�����ۿ۰ٷֱȣ�85Ϊ85�ۣ�
};

//������
struct blacklist {
	char id[32]; //�������绰
};

//����Ա��Ϣ
struct admin {
	time_t set;  //����ʱ��
	time_t geton;    //�����¼ʱ��
	char id[20]; //�˺�
	char password[20];//����	
	char tel[24]; //�绰
};