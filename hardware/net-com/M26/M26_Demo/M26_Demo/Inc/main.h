#ifndef __MAIN_H
#define __MAIN_H

#include <p18f4550.h> 

#define OPENDOOR 				LATCbits.LATC0      //�̵������
#define PWRKEY					LATAbits.LATA3	   //ģ�鿪��
#define DOORSWITCH      		PORTBbits.RB2      //��״̬�����ź�����

#define USART_MAX_RECV_LEN		64		// �����ջ����ֽ���
#define USART_MAX_SEND_LEN		64		// ����ͻ����ֽ���
#define USART_RCV_TIMEOUT     	10   	//���ճ�ʱֵ 10*1ms
#define PWRKEY_PullHight()		PWRKEY=0
#define PWRKEY_PullLow()		PWRKEY=1
#define M26_OK					0

void init(void);				 //ϵͳ��ʼ����������
void M26_Poweron(void);
void USARTRcvTimeout(void);		 //���ڽ��ն�ʱ��ʱ����������
unsigned char SendAlarmNum(unsigned char num, const rom char *ack, unsigned int waittime);//����alarm�������
extern void M26_Init(void);
extern void M26_Poweron(void); 
extern unsigned char M26_CheckCmd(const rom char *str);
extern char* M26_ExtractData(void);
extern void M26_SendString(char *s);

#endif