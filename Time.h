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


//the init method only needs to be called to set up the "timer methods"
void FullTimerInit(float clockspeed);
long GetTimerValue(enum TimeUnits timeUnit);
long GetTotalTimerValueInMilliSeconds();

//this is a partial timer init, if FullTimerInit has been called then this is unncessary
void DelayInit(float clockspeed);
void DelayMicroseconds(int numberOfMicroseconds);
void DelayMilliseconds(int numberOfMilliseconds);//up to 4 seconds
void DelaySeconds(int numberOfSeconds);//up to 16 seconds



float GetClockSpeed();

#endif /* TIME_H_ */