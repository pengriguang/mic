#include "m26.h"


/**
  * @brief	向 M26 模块发送命令
  * @param	cmd	发送的命令
  * @param	ack	期待的应答结果
  * @param	waittime 等待时间(单位:100ms)
  * @retval	0 发送成功
			1 发送失败
  */
unsigned char M26_SendCmd(const rom char *cmd, const rom char *ack, unsigned int waittime)
{
	unsigned char res=0;
	
	RxCount=0;//发送下条命令时，先清接收计数
	while(BusyUSART());
	putrsUSART(cmd);    //发送命令
	if(ack && waittime)
	{
		while(waittime--)//等待waittime*10ms时间
		{
			//Delay10KTCYx(5);//延时10ms
			Delay10KTCYx(50);//延时100ms
			if(RxCount & 0x8000)//数据接收完成
			{		
				if (M26_CheckCmd(ack)) //接收到期待的应答结果
					break;
			}
		}	
		if (waittime == 0)
			res = 1;
	}
	return res;
}

/**
  * @brief	向 M26 模块发送单字节命令
  * @param	cmd	发送的命令字节
  * @param	ack	期待的应答结果
  * @param	waittime 等待时间(单位:100ms)
  * @retval	0 发送成功
			1 发送失败
  */
unsigned char M26_SendByte( unsigned char cmd, const rom char *ack, unsigned int waittime)
{
	unsigned char res=0;
	
	RxCount=0;//发送下条命令时，先清接收计数
	while(BusyUSART());
	WriteUSART(cmd);    //发送命令
	if(ack && waittime)
	{
		while(waittime--)//等待waittime*10ms时间
		{
			Delay10KTCYx(50);//延时100ms
			if(RxCount & 0x8000)//数据接收完成
			{		
				if (M26_CheckCmd(ack)) //接收到期待的应答结果
					break;
			}
		}	
		if (waittime == 0)
			res = 1;
	}
	return res;
}

/**
  * @brief	向 M26 模块发送命令后，检测接收到的应答
  * @param	str		期待的应答结果
  * @retval 0		没有得到期待的应答结果
			1 	
  */
unsigned char M26_CheckCmd(const rom char *str)
{
	char *strx=0;		
	USARTRxBuf[RxCount & 0x7FFF] = 0;//添加结束符
	strx = strstr((const char*)USARTRxBuf, (const char*)str);//判断是否是USARTRxBuf的子字符串
	RxCount = 0;
	if(strx)
		return 1;
	else
		return 0;
}

/**
  * @brief 提取M26接收到的远端数据
  * @retval 返回接收到的字符串
  */
char* M26_ExtractData(void)
{
	char* rcv;
	RxCount=0;
	while(BusyUSART());
	putrsUSART("AT+QIRD=0,1,0,1024\r\n");//发送提取数据指令
	while(!(RxCount & 0x8000));//等待串口接收完成
	USARTRxBuf[RxCount & 0x7FFF] = 0;//添加结束符
	strcpy(rcv,USARTRxBuf);
	return rcv;
}

/**
  *@brief M26模块初始化
  */
void M26_Init(void)
{
//	unsigned char is_ok=1;
	
	M26_SendCmd("ATE0\r\n", "OK", 600);					/* 关闭回显 */
	Delay10KTCYx(50);//延时100ms
//	M26_SendCmd("AT+IPR=9600\r\n", "OK", 600);			/*设置固定波特率为 9600*/
//	Delay10KTCYx(50);//延时100ms
//	M26_SendCmd("AT&W\r\n", "OK", 600);					/*保存配置的固定波特率，即此时模块重启后串口通讯速率为9600*/
//	Delay10KTCYx(50);//延时100ms
//	M26_SendCmd("AT+IPR?", "9600", 600);				/**/
	/* 确保模块工作正常 */
	M26_SendCmd("AT+CPIN?\r\n", "OK", 600);				/* 查询SIM卡状态 */
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+CSQ\r\n", "OK", 600);						/* 信号质量 */
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+CREG?\r\n", "OK", 600);				/* 网络注册信息 */
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+CGREG?\r\n", "OK", 600);				/* GPRS网络注册状态 */
	Delay10KTCYx(50);//延时100ms
	/* TCP设置 */
	M26_SendCmd("AT+QINDI=1\r\n", "OK", 600);           /*接收到数据将会向串口上报提示*/
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIMODE=0\r\n", "OK", 600);			/* 配置TCPIP应用模式为非透传模式 */
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIHEAD=1\r\n", "OK", 600);			/*配置接收数据时显示 IP 头*/
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIFGCNT=0\r\n", "OK", 600);			/*设置当前移动场景*/
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QICSGP=1,\"CMNET\"\r\n", "OK", 600);	/* 设置接入点,联通移动默认CMNET. */
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIDEACT\r\n", "DEACT OK", 600);				/*关闭 GPRS/CSD PDP 场景*/
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIREGAPP\r\n", "OK", 600);			/*启动任务*/
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIACT\r\n", "OK", 600);				/*发起 GPRS/CSD 无线连接*/
	Delay10KTCYx(50);//延时100ms
	while(BusyUSART());
	putrsUSART("AT+QILOCIP\r\n");						/* 获取IP */		
	/* 连接服务器 */
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("ATV1\r\n", "OK", 600);					/*设置 TA 响应返回内容格式,0:返回0，1：返回OK*/
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIDNSIP=1\r\n", "OK", 600);			/*配置使用域名方式连接*/
	Delay10KTCYx(50);//延时100ms
	M26_SendCmd("AT+QIOPEN=\"TCP\",\"cwl0127.xicp.net\",\"40809\"\r\n", "OK", 600);	
	Delay10KTCYx(100);//延时200ms
	
}

/**
  * @param s 待发送的字节内容
  * @param	ack	期待的应答结果
  */
void M26_SendString(char *s)
{
//	unsigned char is_ok=1;
	/* 发送数据到远端服务器 */
	M26_SendCmd("AT+QISEND\r\n", ">", 600);
	while(BusyUSART());
	putsUSART(s);
	M26_SendByte(0x1A,"SEND OK", 600);
}

 