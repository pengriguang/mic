#include "m26.h"


/**
  * @brief	�� M26 ģ�鷢������
  * @param	cmd	���͵�����
  * @param	ack	�ڴ���Ӧ����
  * @param	waittime �ȴ�ʱ��(��λ:100ms)
  * @retval	0 ���ͳɹ�
			1 ����ʧ��
  */
unsigned char M26_SendCmd(const rom char *cmd, const rom char *ack, unsigned int waittime)
{
	unsigned char res=0;
	
	RxCount=0;//������������ʱ��������ռ���
	while(BusyUSART());
	putrsUSART(cmd);    //��������
	if(ack && waittime)
	{
		while(waittime--)//�ȴ�waittime*10msʱ��
		{
			//Delay10KTCYx(5);//��ʱ10ms
			Delay10KTCYx(50);//��ʱ100ms
			if(RxCount & 0x8000)//���ݽ������
			{		
				if (M26_CheckCmd(ack)) //���յ��ڴ���Ӧ����
					break;
			}
		}	
		if (waittime == 0)
			res = 1;
	}
	return res;
}

/**
  * @brief	�� M26 ģ�鷢�͵��ֽ�����
  * @param	cmd	���͵������ֽ�
  * @param	ack	�ڴ���Ӧ����
  * @param	waittime �ȴ�ʱ��(��λ:100ms)
  * @retval	0 ���ͳɹ�
			1 ����ʧ��
  */
unsigned char M26_SendByte( unsigned char cmd, const rom char *ack, unsigned int waittime)
{
	unsigned char res=0;
	
	RxCount=0;//������������ʱ��������ռ���
	while(BusyUSART());
	WriteUSART(cmd);    //��������
	if(ack && waittime)
	{
		while(waittime--)//�ȴ�waittime*10msʱ��
		{
			Delay10KTCYx(50);//��ʱ100ms
			if(RxCount & 0x8000)//���ݽ������
			{		
				if (M26_CheckCmd(ack)) //���յ��ڴ���Ӧ����
					break;
			}
		}	
		if (waittime == 0)
			res = 1;
	}
	return res;
}

/**
  * @brief	�� M26 ģ�鷢������󣬼����յ���Ӧ��
  * @param	str		�ڴ���Ӧ����
  * @retval 0		û�еõ��ڴ���Ӧ����
			1 	
  */
unsigned char M26_CheckCmd(const rom char *str)
{
	char *strx=0;		
	USARTRxBuf[RxCount & 0x7FFF] = 0;//��ӽ�����
	strx = strstr((const char*)USARTRxBuf, (const char*)str);//�ж��Ƿ���USARTRxBuf�����ַ���
	RxCount = 0;
	if(strx)
		return 1;
	else
		return 0;
}

/**
  * @brief ��ȡM26���յ���Զ������
  * @retval ���ؽ��յ����ַ���
  */
char* M26_ExtractData(void)
{
	char* rcv;
	RxCount=0;
	while(BusyUSART());
	putrsUSART("AT+QIRD=0,1,0,1024\r\n");//������ȡ����ָ��
	while(!(RxCount & 0x8000));//�ȴ����ڽ������
	USARTRxBuf[RxCount & 0x7FFF] = 0;//��ӽ�����
	strcpy(rcv,USARTRxBuf);
	return rcv;
}

/**
  *@brief M26ģ���ʼ��
  */
void M26_Init(void)
{
//	unsigned char is_ok=1;
	
	M26_SendCmd("ATE0\r\n", "OK", 600);					/* �رջ��� */
	Delay10KTCYx(50);//��ʱ100ms
//	M26_SendCmd("AT+IPR=9600\r\n", "OK", 600);			/*���ù̶�������Ϊ 9600*/
//	Delay10KTCYx(50);//��ʱ100ms
//	M26_SendCmd("AT&W\r\n", "OK", 600);					/*�������õĹ̶������ʣ�����ʱģ�������󴮿�ͨѶ����Ϊ9600*/
//	Delay10KTCYx(50);//��ʱ100ms
//	M26_SendCmd("AT+IPR?", "9600", 600);				/**/
	/* ȷ��ģ�鹤������ */
	M26_SendCmd("AT+CPIN?\r\n", "OK", 600);				/* ��ѯSIM��״̬ */
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+CSQ\r\n", "OK", 600);						/* �ź����� */
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+CREG?\r\n", "OK", 600);				/* ����ע����Ϣ */
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+CGREG?\r\n", "OK", 600);				/* GPRS����ע��״̬ */
	Delay10KTCYx(50);//��ʱ100ms
	/* TCP���� */
	M26_SendCmd("AT+QINDI=1\r\n", "OK", 600);           /*���յ����ݽ����򴮿��ϱ���ʾ*/
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIMODE=0\r\n", "OK", 600);			/* ����TCPIPӦ��ģʽΪ��͸��ģʽ */
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIHEAD=1\r\n", "OK", 600);			/*���ý�������ʱ��ʾ IP ͷ*/
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIFGCNT=0\r\n", "OK", 600);			/*���õ�ǰ�ƶ�����*/
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QICSGP=1,\"CMNET\"\r\n", "OK", 600);	/* ���ý����,��ͨ�ƶ�Ĭ��CMNET. */
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIDEACT\r\n", "DEACT OK", 600);				/*�ر� GPRS/CSD PDP ����*/
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIREGAPP\r\n", "OK", 600);			/*��������*/
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIACT\r\n", "OK", 600);				/*���� GPRS/CSD ��������*/
	Delay10KTCYx(50);//��ʱ100ms
	while(BusyUSART());
	putrsUSART("AT+QILOCIP\r\n");						/* ��ȡIP */		
	/* ���ӷ����� */
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("ATV1\r\n", "OK", 600);					/*���� TA ��Ӧ�������ݸ�ʽ,0:����0��1������OK*/
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIDNSIP=1\r\n", "OK", 600);			/*����ʹ��������ʽ����*/
	Delay10KTCYx(50);//��ʱ100ms
	M26_SendCmd("AT+QIOPEN=\"TCP\",\"cwl0127.xicp.net\",\"40809\"\r\n", "OK", 600);	
	Delay10KTCYx(100);//��ʱ200ms
	
}

/**
  * @param s �����͵��ֽ�����
  * @param	ack	�ڴ���Ӧ����
  */
void M26_SendString(char *s)
{
//	unsigned char is_ok=1;
	/* �������ݵ�Զ�˷����� */
	M26_SendCmd("AT+QISEND\r\n", ">", 600);
	while(BusyUSART());
	putsUSART(s);
	M26_SendByte(0x1A,"SEND OK", 600);
}

 