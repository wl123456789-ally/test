#include "pid.h"



struct _pid 
{
	float ExpectedValue;//�����趨ֵ
	float ActualValue;//����ʵ��ֵ
	float err;//����ƫ��ֵ
	float err_last;//������һ��ƫ��ֵ
	float err_prev;//����ǰһ����ƫ��ֵ
	float Kp, Ki, Kd;//������������֡�΢��ϵ��
}pid;

 
void PID_Init(void)
{
	pid.ExpectedValue = 0.0;
	pid.ActualValue = 0.0;
	pid.err = 0.0;
	pid.err_prev = 0.0;
	pid.err_last = 0.0;
	pid.Kp = 0.4;
	pid.Ki = 0.2;
	pid.Kd = 0.0;
}
 
float PID_Realize(float speed) 
{
//	float index;
  float incrementValue;
	pid.ExpectedValue = speed;
	pid.err = pid.ExpectedValue - pid.ActualValue;
	//����ʽpid��ʽ
  incrementValue = pid.Kp*(pid.err - pid.err_last) + pid.Ki*pid.err + pid.Kd*(pid.err - 2 * pid.err_last + pid.err_prev);
	pid.ActualValue += incrementValue;
	pid.err_prev = pid.err_last;
	pid.err_last = pid.err;
	return pid.ActualValue;
}

