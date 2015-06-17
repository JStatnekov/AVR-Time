/*
 * Time.h
 *
 * Created: 10/5/2012 12:50:18 AM
 *  Author: JBS
 */ 


#ifndef TIME_H_
#define TIME_H_



enum TimeUnits
{
	TensOfMilliSeconds,
	Seconds,
	Minutes
};



void TimerInit(float clockspeed);

void DelayMicroseconds(int numberOfMicroseconds);
void DelayMilliseconds(int numberOfMilliseconds);//up to 4 seconds
void DelaySeconds(int numberOfSeconds);//up to 16 seconds
int GetTime(enum TimeUnits timeUnit);
long GetNumberOfMilliSecondsSinceStart();

float GetClockSpeed();

#endif /* TIME_H_ */