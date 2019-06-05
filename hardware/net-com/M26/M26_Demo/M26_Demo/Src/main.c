#include <usart.h>
#include <timers.h>  
#include <string.h>
#include "main.h"                  
#include "p18f4550config.h"
#include "user_delays.h"
#include "m26.h"


//#pragma udata buffer_scn                 //重定义接收缓冲区大小
//#pragma udata

char USARTRxBuf[USART_MAX_RECV_LEN];//接收缓冲区
char *USARTTxBuf;
char TxBuf[4]={0x99,0xB1,0x0,0x0A};
//const rom char tes[]="www.sensertek.com\r\n";
unsigned int RxCount=0;  					//接收计数
unsigned char USARTRcvTimeoutCnt=0;  		//接收定时计数
unsigned char USARTRcvFlag=0;				//接收到数据标记
unsigned char open_flag=0;					//状态标记位

void InterruptHandlerHigh(void); //高优先级中断函数申明
void InterruptHandlerLow(void);  //低优先级中断函数申明

void init(void)
{

	WDTCONbits.SWDTEN=1;    
	TRISAbits.TRISA0=0; //BUSY输出
	TRISAbits.TRISA2=1; //HAND输入  
	TRISAbits.TRISA3=0; //PWRKEY输出   
	TRISAbits.TRISA4=0; //STCRESET,输出
	TRISBbits.TRISB2=1; //门状态信号输入
	TRISCbits.TRISC0=0; //继电器输出（开门）
	TRISCbits.TRISC1=0; //继电器输出（关门）                
	TRISD=0;            //PORTD为输出口
	TRISEbits.TRISE0=1; //AISLESWITCH方向输入      
    TRISEbits.TRISE1=1; //CASHERSWITCH方向输入
    TRISEbits.TRISE2=1; //EASSWITCH方向输入
	ADCON1=0x0F;        //设置所有的I/O为数字口
	CMCON=0x07; 
	UCONbits.USBEN=0;   //RC4 作为普通I/O
	UCFGbits.UTRDIS=1;
	OPENDOOR=0;
	PWRKEY=0;
	RCONbits.IPEN=1;   //中断优先使能
	INTCONbits.GIEH=1; //允许所有高优先级中断
	INTCONbits.GIEL=1; //允许所有低优先级的外设中断
	IPR1bits.RCIP=1;   //EUSART接收中断高优先级
	INTCON2bits.TMR0IP =0;  //Timer0定时中断低优先级
	
	OpenTimer0( TIMER_INT_ON &   //允许Timer0中断
				T0_16BIT &       //16位模式
				T0_SOURCE_INT &  //内部时钟源
				T0_PS_1_8 );	 //1：8预分频 
	WriteTimer0(0xFD8E);//定时：(0xFFFF-0xFD8E)*(4/20)*8us=1ms
				
	OpenUSART( USART_TX_INT_OFF & 
			   USART_RX_INT_ON & 
               USART_ASYNCH_MODE & 
               USART_EIGHT_BIT & 
               USART_CONT_RX & 
               USART_BRGH_HIGH,129 );      //波特率：9600(129)，19200(64),38400(32),57600(21)@20MHz  波特率=TOSC/(16*(X+1))  
               	                      
}


/**
  * @brief	M26 模块开机
  */
void M26_Poweron(void)
{
	/* 手动开机*/
	
//	PWRKEY_PullLow();
//	Delay_s(2);
//	PWRKEY_PullHight();
//	if(M26_SendCmd("AT\r\n", "OK", 600)!=M26_OK)//如果开机失败
//		Reset();

	/* 上电等待模块自启动 */
	Delay_s(5);
}

/**
  * @brief 串口接收定时超时处理
  */
void USARTRcvTimeout(void)
{
	RxCount |= 0x8000;//接收完成
	USARTRcvFlag = 0;//关闭定时
}

/**
  * @brief 发送alarm报警编号
  * @param num 报警端口号
  * @param ack 期待的应答结果
  * @param waittime 等待时间(单位:100ms)
  * @retval 0 发送成功
  	 		1 发送失败
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

void main(void)
{
    init();
	M26_Poweron();
	M26_Init();
	
	/* 发送数据测试 */
//	strcpypgm2ram(USARTTxBuf, "www.sensertek.com\r\n");//把程序存储区的字符串tes拷贝到数据存储区
//	M26_SendString(USARTTxBuf);
//	OPENDOOR=~OPENDOOR;
    while(1)
    {   
		ClrWdt();
		if(RxCount & 0x8000)//数据接收完成
		{
			if(M26_CheckCmd("+QIRDI: 0,1,0"))//表明接收到模块发来的远端数据
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
	if(PIR1bits.RCIF)//串口接收数据
	{
		PIR1bits.RCIF=0;
		if(RxCount<USART_MAX_RECV_LEN)
		{
			USARTRcvFlag = 1;    //USARTRcvFlag=1表示接收到数据
			USARTRcvTimeoutCnt=0;//接收到新数据，定时计数清0
			USARTRxBuf[RxCount++]=ReadUSART();//把接收到的字节存储到接收缓冲区	
		}
		else
		{
			/* 接收完成 */
			RxCount |= 1<<15; //USARTRxSta最高位置1
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
		if ((USARTRcvFlag == 1) && (USARTRcvTimeoutCnt >= USART_RCV_TIMEOUT))//10ms没有接收到数据，表示数据接收完成
		{
			USARTRcvTimeout();
		}
	}
}