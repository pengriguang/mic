#include <usart.h>
#include <timers.h>  
#include <string.h>
#include "main.h"                  
#include "p18f4550config.h"
#include "user_delays.h"
#include "m26.h"


//#pragma udata buffer_scn                 //�ض�����ջ�������С
//#pragma udata

char USARTRxBuf[USART_MAX_RECV_LEN];//���ջ�����
char *USARTTxBuf;
char TxBuf[4]={0x99,0xB1,0x0,0x0A};
//const rom char tes[]="www.sensertek.com\r\n";
unsigned int RxCount=0;  					//���ռ���
unsigned char USARTRcvTimeoutCnt=0;  		//���ն�ʱ����
unsigned char USARTRcvFlag=0;				//���յ����ݱ��
unsigned char open_flag=0;					//״̬���λ

void InterruptHandlerHigh(void); //�����ȼ��жϺ�������
void InterruptHandlerLow(void);  //�����ȼ��жϺ�������

void init(void)
{

	WDTCONbits.SWDTEN=1;    
	TRISAbits.TRISA0=0; //BUSY���
	TRISAbits.TRISA2=1; //HAND����  
	TRISAbits.TRISA3=0; //PWRKEY���   
	TRISAbits.TRISA4=0; //STCRESET,���
	TRISBbits.TRISB2=1; //��״̬�ź�����
	TRISCbits.TRISC0=0; //�̵�����������ţ�
	TRISCbits.TRISC1=0; //�̵�����������ţ�                
	TRISD=0;            //PORTDΪ�����
	TRISEbits.TRISE0=1; //AISLESWITCH��������      
    TRISEbits.TRISE1=1; //CASHERSWITCH��������
    TRISEbits.TRISE2=1; //EASSWITCH��������
	ADCON1=0x0F;        //�������е�I/OΪ���ֿ�
	CMCON=0x07; 
	UCONbits.USBEN=0;   //RC4 ��Ϊ��ͨI/O
	UCFGbits.UTRDIS=1;
	OPENDOOR=0;
	PWRKEY=0;
	RCONbits.IPEN=1;   //�ж�����ʹ��
	INTCONbits.GIEH=1; //�������и����ȼ��ж�
	INTCONbits.GIEL=1; //�������е����ȼ��������ж�
	IPR1bits.RCIP=1;   //EUSART�����жϸ����ȼ�
	INTCON2bits.TMR0IP =0;  //Timer0��ʱ�жϵ����ȼ�
	
	OpenTimer0( TIMER_INT_ON &   //����Timer0�ж�
				T0_16BIT &       //16λģʽ
				T0_SOURCE_INT &  //�ڲ�ʱ��Դ
				T0_PS_1_8 );	 //1��8Ԥ��Ƶ 
	WriteTimer0(0xFD8E);//��ʱ��(0xFFFF-0xFD8E)*(4/20)*8us=1ms
				
	OpenUSART( USART_TX_INT_OFF & 
			   USART_RX_INT_ON & 
               USART_ASYNCH_MODE & 
               USART_EIGHT_BIT & 
               USART_CONT_RX & 
               USART_BRGH_HIGH,129 );      //�����ʣ�9600(129)��19200(64),38400(32),57600(21)@20MHz  ������=TOSC/(16*(X+1))  
               	                      
}


/**
  * @brief	M26 ģ�鿪��
  */
void M26_Poweron(void)
{
	/* �ֶ�����*/
	
//	PWRKEY_PullLow();
//	Delay_s(2);
//	PWRKEY_PullHight();
//	if(M26_SendCmd("AT\r\n", "OK", 600)!=M26_OK)//�������ʧ��
//		Reset();

	/* �ϵ�ȴ�ģ�������� */
	Delay_s(5);
}

/**
  * @brief ���ڽ��ն�ʱ��ʱ����
  */
void USARTRcvTimeout(void)
{
	RxCount |= 0x8000;//�������
	USARTRcvFlag = 0;//�رն�ʱ
}

/**
  * @brief ����alarm�������
  * @param num �����˿ں�
  * @param ack �ڴ���Ӧ����
  * @param waittime �ȴ�ʱ��(��λ:100ms)
  * @retval 0 ���ͳɹ�
  	 		1 ����ʧ��
  */
unsigned char SendAlarmNum(unsigned char num, const rom char *ack, unsigned int waittime)
{
	unsigned char res=0;
	TxBuf[2]=num;
	M26_SendCmd("AT+QISEND=4\r\n", ">", 600);
	while(BusyUSART());
	putsUSART(TxBuf);
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

void main(void)
{
    init();
	M26_Poweron();
	M26_Init();
	
	/* �������ݲ��� */
//	strcpypgm2ram(USARTTxBuf, "www.sensertek.com\r\n");//�ѳ���洢�����ַ���tes���������ݴ洢��
//	M26_SendString(USARTTxBuf);
//	OPENDOOR=~OPENDOOR;
    while(1)
    {   
		ClrWdt();
		if(RxCount & 0x8000)//���ݽ������
		{
			if(M26_CheckCmd("+QIRDI: 0,1,0"))//�������յ�ģ�鷢����Զ������
			{
				USARTTxBuf = M26_ExtractData();
//				M26_SendString(USARTTxBuf);
				OPENDOOR=~OPENDOOR;	
			}
			RxCount=0;
		}

		if(DOORSWITCH==1)
		{
			open_flag=1;
		}
		if(DOORSWITCH==0&&open_flag==1)
		{
			open_flag=0;
			SendAlarmNum(0x01, "OK", 200);
		}

	}
}
 
 
#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
       _asm
           goto InterruptHandlerHigh    //jump to interrupt routine
       _endasm
}
#pragma code
#pragma interrupt InterruptHandlerHigh
void InterruptHandlerHigh (void)         
{      
	if(PIR1bits.RCIF)//���ڽ�������
	{
		PIR1bits.RCIF=0;
		if(RxCount<USART_MAX_RECV_LEN)
		{
			USARTRcvFlag = 1;    //USARTRcvFlag=1��ʾ���յ�����
			USARTRcvTimeoutCnt=0;//���յ������ݣ���ʱ������0
			USARTRxBuf[RxCount++]=ReadUSART();//�ѽ��յ����ֽڴ洢�����ջ�����	
		}
		else
		{
			/* ������� */
			RxCount |= 1<<15; //USARTRxSta���λ��1
		}
              
	} 	
} 

#pragma code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
       _asm
           goto InterruptHandlerLow    //jump to interrupt routine
       _endasm
}
#pragma code

#pragma interruptlow InterruptHandlerLow
void InterruptHandlerLow(void)
{
	if(INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF=0;
		WriteTimer0(0xFD8E);
		USARTRcvTimeoutCnt++;
		if ((USARTRcvFlag == 1) && (USARTRcvTimeoutCnt >= USART_RCV_TIMEOUT))//10msû�н��յ����ݣ���ʾ���ݽ������
		{
			USARTRcvTimeout();
		}
	}
}