#include "pid.h"



struct _pid 
{
	float ExpectedValue;//定义设定值
	float ActualValue;//定义实际值
	float err;//定义偏差值
	float err_last;//定义上一个偏差值
	float err_prev;//定义前一个的偏差值
	float Kp, Ki, Kd;//定义比例、积分、微分系数
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
	//增量式pid公式
  incrementValue = pid.Kp*(pid.err - pid.err_last) + pid.Ki*pid.err + pid.Kd*(pid.err - 2 * pid.err_last + pid.err_prev);
	pid.ActualValue += incrementValue;
	pid.err_prev = pid.err_last;
	pid.err_last = pid.err;
	return pid.ActualValue;
}

