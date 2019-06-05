#ifndef __M26_H
#define __M26_H

#include "user_delays.h"
#include "main.h"
#include <usart.h>
#include <string.h>

extern unsigned int  RxCount;  //���ռ���
extern char USARTRxBuf[USART_MAX_RECV_LEN];//���ջ�����


unsigned char M26_SendByte(unsigned char cmd, const rom char *ack, unsigned int waittime);

/**
  * @brief	�� M26 ģ�鷢������
  * @param	cmd	���͵�����
  * @param	ack	�ڴ���Ӧ����
  * @param	waittime �ȴ�ʱ��(��λ:10ms)
  * @retval	0 ���ͳɹ�
			1 ����ʧ��
  */
unsigned char M26_SendCmd(const rom char *cmd, const rom char *ack, unsigned int waittime);//�� M26 ģ�鷢������

/**
  * @brief	�� M26 ģ�鷢������󣬼����յ���Ӧ��
  * @param	str		�ڴ���Ӧ����
  * @retval 0		û�еõ��ڴ���Ӧ����
			���� 	�ڴ�Ӧ������λ��
  */
unsigned char M26_CheckCmd(const rom char *str);

/**
  * @brief ��ȡM26���յ���Զ������
  * @retval ���ؽ��յ����ַ���
  */
char* M26_ExtractData(void);

/**
  *@brief M26ģ���ʼ��
  */
void M26_Init(void);

/**
  * @param s �����͵��ֽ�����
  */
void M26_SendString(char *s);

/* @param	ByteCount �����͵��ֽ���
 */

#endif