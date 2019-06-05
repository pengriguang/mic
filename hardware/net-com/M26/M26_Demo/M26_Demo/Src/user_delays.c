#include "user_delays.h"

/**
  * @brief	秒级延时函数
  * @param Count 期望延时的秒数
  */
void Delay_s(unsigned int Count)
{
	unsigned int i;
	for(i=Count;i>0;i--)//延时Count s
	{
		Delay10KTCYx(250);//延时500ms
		Delay10KTCYx(250);
	}
}