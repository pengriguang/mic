#ifndef __MAIN_H
#define __MAIN_H

#include <p18f4550.h> 

#define OPENDOOR 				LATCbits.LATC0      //继电器输出
#define PWRKEY					LATAbits.LATA3	   //模块开机
#define DOORSWITCH      		PORTBbits.RB2      //门状态关门信号输入

#define USART_MAX_RECV_LEN		64		// 最大接收缓存字节数
#define USART_MAX_SEND_LEN		64		// 最大发送缓存字节数
#define USART_RCV_TIMEOUT     	10   	//接收超时值 10*1ms
#define PWRKEY_PullHight()		PWRKEY=0
#define PWRKEY_PullLow()		PWRKEY=1
#define M26_OK					0

void init(void);				 //系统初始化函数申明
void M26_Poweron(void);
void USARTRcvTimeout(void);		 //串口接收定时超时处理函数申明
unsigned char SendAlarmNum(unsigned char num, const rom char *ack, unsigned int waittime);//发送alarm报警编号
extern void M26_Init(void);
extern void M26_Poweron(void); 
extern unsigned char M26_CheckCmd(const rom char *str);
extern char* M26_ExtractData(void);
extern void M26_SendString(char *s);

#endif