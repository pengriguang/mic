#ifndef __M26_H
#define __M26_H

#include "user_delays.h"
#include "main.h"
#include <usart.h>
#include <string.h>

extern unsigned int  RxCount;  //接收计数
extern char USARTRxBuf[USART_MAX_RECV_LEN];//接收缓冲区


unsigned char M26_SendByte(unsigned char cmd, const rom char *ack, unsigned int waittime);

/**
  * @brief	向 M26 模块发送命令
  * @param	cmd	发送的命令
  * @param	ack	期待的应答结果
  * @param	waittime 等待时间(单位:10ms)
  * @retval	0 发送成功
			1 发送失败
  */
unsigned char M26_SendCmd(const rom char *cmd, const rom char *ack, unsigned int waittime);//向 M26 模块发送命令

/**
  * @brief	向 M26 模块发送命令后，检测接收到的应答
  * @param	str		期待的应答结果
  * @retval 0		没有得到期待的应答结果
			其他 	期待应答结果的位置
  */
unsigned char M26_CheckCmd(const rom char *str);

/**
  * @brief 提取M26接收到的远端数据
  * @retval 返回接收到的字符串
  */
char* M26_ExtractData(void);

/**
  *@brief M26模块初始化
  */
void M26_Init(void);

/**
  * @param s 待发送的字节内容
  */
void M26_SendString(char *s);

/* @param	ByteCount 待发送的字节数
 */

#endif