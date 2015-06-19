#include <avr/io.h>
#include "Time.h"


//this tests the timer functionality by toggling a diode on port c5
int main(void){
	DDRC |= (1<<PORTC5);

	DelayInit(16e6);
	while(1)
	{
		DelayMilliseconds(500);
		PORTC ^= (1<<PORTC5);
	}
}