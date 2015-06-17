/*
 * Time.c
 *
 * Created: 10/6/2012 3:25:44 PM
 *  Author: JBS
 */ 
#include "Time.h"
#include <avr/interrupt.h>

static volatile long _timeTensOfMilliSeconds;
static volatile long _timeSeconds;
static volatile long _timeMinutes;
static uint8_t initializedAlready = 0;

//TODO: set this according to whatever clock speed you are using
float _clockSpeed;

float GetClockSpeed()
{
	return _clockSpeed;
};


void TimerInit(float clockSpeed)
{
	if(initializedAlready)
		return;
	
	_clockSpeed = clockSpeed;
	initializedAlready = 1;	
	_timeTensOfMilliSeconds = 0;
	_timeSeconds = 0;
	_timeMinutes = 0;
	
	cli();
	TCCR0A |= (1<<WGM01);
	TIMSK0 |= (1 << OCIE0A);//enable interrupt comparing with OCROA
	
	TCCR0B &= ~((1 << CS02) |(1<<CS01)| (1<< CS22));//clear any previously set values (TimerInit might be called to recalibrate the time after drift has occured)
	
	if(_clockSpeed == (float)1e6)
	{
		TCCR0B |= (1 << CS02);//prescale by 256

		OCR0A = 39;//every 3,906.25 ticks make up a second
		//we lose 0.0016 of a second every second, after 10min 25s we have lost a second (our clock is fast by a second)
	}
	else if(_clockSpeed == (float)16e6)
	{
		TCCR0B |= (1 << CS02) | (1<< CS22);//prescale by 1024
		OCR0A = 156;//every 15,625 ticks make up a second
		//we lose 0.0016 of a second every second, after 10min 25s we have lost a second (our clock is fast by a second)
	}
	else if(_clockSpeed == (float)14.7456e6)
	{
		TCCR0B |= (1 << CS02) | (1<< CS22);//prescale by 1024
		 OCR0A = 144;//every 14,400 ticks make up a second
		//we lose nothing, the clock is accurate
	}
	else if(_clockSpeed == (float)12.288e6)
	{
		TCCR0B |= (1 << CS02) | (1<< CS22);//prescale by 1024
		OCR0A = 120;//every 12000 ticks make up a second
		//we lose nothing, the clock is accurate
	}

    TCNT0 = 0;
	// enable global interrupts
    sei();
};

//This will not have very good results with a clock slower than 10Mhz
//and is only useful for rough estimates. For small wait times, it is useless.
void DelayMicroseconds(int numberOfMicroseconds)
{
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));//clear any previously set flags
	float numberOfTicksToWait = 0;
	
	int numberOfTicksConsumedinSetup = 14;

	if(_clockSpeed == (float)1e6)
	{//at 1,000,000 ticks per second, there is 1 tick per uS
		numberOfTicksToWait = numberOfMicroseconds;
	}
	else if(_clockSpeed == (float)16e6)
	{//at 16,000,000 ticks per second, there are 16 ticks per uS
		numberOfTicksToWait = numberOfMicroseconds*16;
		numberOfTicksConsumedinSetup++;
	}
	else if(_clockSpeed == (float)14.7456e6)
	{//14,745,600 ticks per second, there are 14.7456 ticks per uS
		numberOfTicksToWait = numberOfMicroseconds * (float)14.7456;
		numberOfTicksConsumedinSetup +=2;
	}
	else if(_clockSpeed == (float)12.288e6)
	{//12,288,000 ticks per second, there are 12.288 ticks per uS
		numberOfTicksToWait = numberOfMicroseconds * (float)12.288e6;
		numberOfTicksConsumedinSetup +=3;
	}

	//TCNT1 is 2bytes in size
	TCNT1 = 0; // Reset timer value

	//I estimate that it has taken x ticks to get up to this point, and will take a tick to get back to the calling code
	while(TCNT1 < (numberOfTicksToWait - numberOfTicksConsumedinSetup))
	{}
	
};

void DelayMilliseconds(int numberOfMilliseconds)//up to 4.1 seconds
{
	TCCR1B &= ~((1<<CS12)|(1<<CS11)| (1<<CS10));//clear any previously set flags
	float numberOfTicksToWait = 0;

	if(_clockSpeed == (float)1e6)
	{//at 1,000,000 ticks per second, there are 1,000 ticks per milliSecond
		//if we prescale by 64 then every 15.625ticks is equal to a milliSecond
		//the max value that TCNT1 can hold is 65,536 the max delay is therefore 4194.304 seconds
		TCCR1B = (1 << CS11) | (1 << CS10);
		numberOfTicksToWait = 15.625*numberOfMilliseconds;
	}
	else if(_clockSpeed == (float)16e6)
	{//at 16,000,000 ticks per second, there are 16,000 ticks per milliSecond
		//if we prescale by 1024 then every 15.625ticks is equal to a milliSecond
		//the max value that TCNT1 can hold is 65,536 the max delay is therefore 4194.304 seconds
		TCCR1B = (1 << CS12)| (1<<CS10);
		numberOfTicksToWait = 15.625*numberOfMilliseconds;
	}
	else if(_clockSpeed == (float)14.7456e6)
	{//14,745,600 ticks per second, there are 14,745.6 ticks per milliSecond
		//if we prescale by 1024 then every 14.4ticks is equal to a milliSecond
		//the max value that TCNT1 can hold is 65,536 the max delay is therefore 4551.111seconds
		TCCR1B = (1 << CS12) | (1<<CS10);
		numberOfTicksToWait = 14.4*numberOfMilliseconds;
	}
	else if(_clockSpeed == (float)12.288e6)
	{//12,288,000 ticks per second, there are 12,288 ticks per milliSecond
		//if we prescale by 1024 then every 12ticks is equal to a milliSecond
		//the max value that TCNT1 can hold is 65,536 the max delay is therefore 5461.333 seconds
		TCCR1B = (1 << CS12)| (1<<CS10);
		numberOfTicksToWait = 12*numberOfMilliseconds;
	}

	//TCNT1 is 2bytes in size
	TCNT1 = 0; // Reset timer value
	while(TCNT1 < numberOfTicksToWait)
	{}
};

void DelaySeconds(int numberOfSeconds)//up to 16 seconds
{
	for(int i = 0; i < numberOfSeconds; ++i){DelayMilliseconds(1000);}
};

long GetNumberOfMilliSecondsSinceStart()
{
	cli();
	long time = (_timeMinutes*60*1000)+(_timeSeconds*1000)+(_timeTensOfMilliSeconds*10);
	sei();
	return time;
};

int GetTime(enum TimeUnits timeUnit)
{	
	cli();
	uint8_t returnValue = 0;
	switch(timeUnit)
	{
		case TensOfMilliSeconds: 
			returnValue = _timeTensOfMilliSeconds; 
			break;
		case Seconds:
			returnValue = _timeSeconds; 
			break;
		case Minutes:
			returnValue = _timeMinutes; 
			break;
	}
	sei();
	return returnValue;
};


ISR(TIMER0_COMPA_vect)
{
	_timeTensOfMilliSeconds++;
	
	if(_timeTensOfMilliSeconds >= 100)
	{
		_timeTensOfMilliSeconds = 0;
		_timeSeconds++;
		if(_timeSeconds >= 60)
		{
			_timeSeconds=0;
			_timeMinutes++;
		}
	}	
};

