#include "user_delays.h"

/**
  * @brief	�뼶��ʱ����
  * @param Count ������ʱ������
  */
void Delay_s(unsigned int Count)
{
	unsigned int i;
	for(i=Count;i>0;i--)//��ʱCount s
	{
		Delay10KTCYx(250);//��ʱ500ms
		Delay10KTCYx(250);
	}
}