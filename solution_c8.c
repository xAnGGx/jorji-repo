#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
int main() {
	DDRA = 0xFF;
	DDRC = 0xFF;
	PORTA = 0x00;
	PORTC = 0x00;
	PORTC &= 0b11111110;
	PORTC |= 0b00000100;
	PORTA = 0b00001100;
	PORTC &= 0b11111011;
	_delay_us(20);
	PORTC &= 0b11111110;
	PORTC |= 0b00000100;
	PORTA = 0b00000110;
	PORTC &= 0b11111011;
	_delay_us(40);
	char *str = "Hello World!";
	for (int i = 0; str[i]; i++) {
		PORTC |= 0b00000001;
		PORTC |= 0b00000100;
		PORTA = str[i];
		PORTC &= 0b11111011;
		_delay_us(50);
	}
	PORTC &= 0b11111110;
	PORTC |= 0b00000100;
	PORTA = 0b00100000;
	PORTC &= 0b11111011;
	_delay_us(50);
	while (1) {
		continue;
	}
}
