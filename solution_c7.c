#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
int main() {
	DDRA = 0x80;
	DDRB = 0xFF;
	PORTA = 0x7F;
	PORTB = 0xAA;
	unsigned int cntNew = 0, cntCur = 0;
	while (1) {
		cntCur = cntNew % 128;
		PORTA |= 0b10000000;
		_delay_ms(1);
		PORTA &= 0b01111111;
		cntNew = (unsigned char)(PINA & 0x7F);
		if (cntNew < cntCur) {
			cntNew += 128;
		}
		if (cntNew - cntCur >= 24) {
			PORTB = 0xFF;
		} else {
			PORTB = 0x00;
		}
	}
}
