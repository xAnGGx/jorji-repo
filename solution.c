#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
int main() {
	short up;
	short cur = 0x01;
	DDRA = 0xFF;
	DDRB = 0x00;
	PORTB = 0xFF;
	while (true) {
		PORTA = cur;
		_delay_ms(500);
		up = PINB;
		if (up)
			cur = (cur << 1) | (cur >> 7);
		else
			cur = (cur >> 1) | (cur << 7);
	}
}
