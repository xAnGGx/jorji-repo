#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
int main() {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	unsigned int num = PINA;
	switch(num) {
		case 0:
			PORTB = 0b00111111;
			break;
		case 1:
			PORTB = 0b00000110;
			break;
		case 2:
			PORTB = 0b01011011;
			break;
		case 3:
			PORTB = 0b01001111;
			break;
		case 4:
			PORTB = 0b01100110;
			break;
		case 5:
			PORTB = 0b01101101;
			break;
		case 6:
			PORTB = 0b01111101;
			break;
		case 7:
			PORTB = 0b00000111;
			break;
		case 8:
			PORTB = 0b01111111;
			break;
		case 9:
			PORTB = 0b01101111;
			break;
	}
	while (1) {
		continue;
	}
}
