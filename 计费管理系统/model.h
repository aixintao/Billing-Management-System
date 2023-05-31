#pragma once
#include <time.h>
//卡结构体
struct Card {
	time_t set;  //创建时间
	time_t geton;    //上机时间
	time_t getoff;   //下机时间
	char id[20]; //卡号
	char password[20];//密码	
	char tel[24]; //电话
	int balance; //余额，这里表示多少分钱，防止精度造成问题
	int credit; //信用值
	int vip; //会员等级
	int on; //是否在上机中
	int usecount; //使用次数
	int del; //删除标志
};

//计费标准（分档计费制 v0.2.1加入
struct standard {
	char name[18]; //名称
	double gap; //满足标准（……分钟以上时）
	int minute; //每……分钟
	double money; //……元
	double rate; //每分钟花费（自动计算）
	int del; //删除标记
};

//通用事件日志
struct log {
	char id[20]; //相关卡号/名称
	int type; //事件类型
	// 0
	// -1 管理员登录
	// -2 管理员改密码
	// -3 管理员登出
	// -4 管理员被删除
	// -10 计费标准新增
	// -12 计费标准修改
	// -14 计费标准删除
	// -20 信用低列入黑名单
	// -21 手机号失信卡注销
	// -24 黑名单删除
	// -50 会员等级增加
	// -52 会员等级修改
	// -54 会员等级删除
	// -59 管理员密码错误过多
	// -998 系统启动
	// -999 系统关闭
	// 1 上机
	// 2 下机
	// 3 充值
	// 4 退费
	// 10 添加卡
	// 12 改密码
	// 14 删除卡
	// 20 卡密码错误过多
	// 30 晋升会员
	int amount; //相关金额
	int balance;//相关余额
	int gap; //相关满足条件
	int gap_pre; //相关原满足条件
	int duration; //相关上机时长
	int duration_pre; //相关原上机时长
	int amount_pre; //相关原金额
	time_t time;//相关时间
	char tel[24]; //相关手机号
	char vip[16];//相关会员头衔
	char vipcolor; //相关会员颜色
	char vip_pre[16];//相关原会员头衔
	char vipcolor_pre; //相关原会员颜色
	int del; //删除标志
};

//会员等级
struct vip {
	char id[16]; //会员等级名
	int gap; //一次性充值（单位）可享受该vip
	int color; //会员高亮显示色
	int discount; //所享折扣百分比（85为85折）
};

//黑名单
struct blacklist {
	char id[32]; //黑名单电话
};

//管理员信息
struct admin {
	time_t set;  //创建时间
	time_t geton;    //最近登录时间
	char id[20]; //账号
	char password[20];//密码	
	char tel[24]; //电话
};