#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
int main() {
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;
	DDRE = 0xFF;
	PORTA = 0xFF;
	PORTB = 0xFF;
	PORTC = 0xFF;
	PORTD = 0xFF;
	unsigned char n = PINA;
	unsigned char x = PINB;
	unsigned char y = PINC;
	unsigned char z = PIND;
	n &= 0b11011011;
	n |= 0b10001000;
	unsigned char rev = 0;
	int i;
	for (i = 0; i < 8; i++) { 		
		rev |= (n & 1);
		rev = (rev << 1) | (rev >> 7);		
		n = n >> 1;
	}
	rev = (rev >> 1) | (rev << 7);
	rev |= (unsigned char)(1 << y);
	rev &= (unsigned char)(~(1 << z));
	unsigned char ans = 0;
	for (i = 0; i < 8; i++) {
		if (x & 1) {
			ans |= (rev & 1);
		}
		else {
			ans |= ((~rev) & 1);
		}
		x = x >> 1;
		rev = rev >> 1;
		ans = (ans >> 1) | (ans << 7);
	}
	int fin = 0;
	for (i = 0; i < 8; i++) {
		if (ans & 1) fin++;
		ans = ans >> 1;
	}
	switch(fin) {
		case 0:
			PORTE = 0b00111111;
			break;
		case 1:
			PORTE = 0b00000110;
			break;
		case 2:
			PORTE = 0b01011011;
			break;
		case 3:
			PORTE = 0b01001111;
			break;
		case 4:
			PORTE = 0b01100110;
			break;
		case 5:
			PORTE = 0b01101101;
			break;
		case 6:
			PORTE = 0b01111101;
			break;
		case 7:
			PORTE = 0b00000111;
			break;
		case 8:
			PORTE = 0b01111111;
			break;
		case 9:
			PORTE = 0b01101111;
			break;
	}
	while (1) {
		continue;
	}
}
