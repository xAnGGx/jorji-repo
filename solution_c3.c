#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
int main() {
	int goUp = 1;
	unsigned char up, upNew;
	unsigned char cur = 0x01;
	DDRA = 0xFF;
	DDRB = 0x00;
	PORTB = 0xFF;
	PORTA = cur;
	up = PINB & 0x01;
	_delay_ms(100);
	while (1) {
		upNew = PINB & 0x01;
		if (up != upNew) goUp = (goUp + 1) % 2;
		up = upNew;
		if (goUp)
			cur = (cur << 1) | (cur >> 7);
		else
			cur = (cur >> 1) | (cur << 7);
		PORTA = cur;
		_delay_ms(100);
	}
}
