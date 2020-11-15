#ifndef F_CPU
#define 4000000UL
#endif
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
volatile unsigned int ms_passed = 0;
void output_time(unsigned int num) {
	switch(num%10) {
		case 0:
			PORTC = 0b00111111;
			break;
		case 1:
			PORTC = 0b00000110;
			break;
		case 2:
			PORTC = 0b01011011;
			break;
		case 3:
			PORTC = 0b01001111;
			break;
		case 4:
			PORTC = 0b01100110;
			break;
		case 5:
			PORTC = 0b01101101;
			break;
		case 6:
			PORTC = 0b01111101;
			break;
		case 7:
			PORTC = 0b00000111;
			break;
		case 8:
			PORTC = 0b01111111;
			break;
		case 9:
			PORTC = 0b01101111;
			break;
	}
	switch((num/10)%10) {
		case 0:
			PORTD = 0b00111111;
			break;
		case 1:
			PORTD = 0b00000110;
			break;
		case 2:
			PORTD = 0b01011011;
			break;
		case 3:
			PORTD = 0b01001111;
			break;
		case 4:
			PORTD = 0b01100110;
			break;
		case 5:
			PORTD = 0b01101101;
			break;
		case 6:
			PORTD = 0b01111101;
			break;
		case 7:
			PORTD = 0b00000111;
			break;
		case 8:
			PORTD = 0b01111111;
			break;
		case 9:
			PORTD = 0b01101111;
			break;
	}
	switch((num/100)%10) {
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
}
ISR(TIMER0_COMPA_vect) {
	ms_passed++;
	output_time(ms_passed % 1000);
}
int main() {
	DDRA = 0xFF;
	PORTA = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0xFF;
	DDRD = 0xFF;
	DDRE = 0xFF;
	output_time(ms_passed);
	TCNT0 = 0;
	OCR0A = 62;
	TCCR0B = (1 << CS01) | (1 << CS00);
	TCCR0A = (1 << WGM01);
	TIMSK0 = (1 << OCIE0A);
	sei();
	unsigned short a = eeprom_read_word((uint16_t*)0);
	unsigned short b = eeprom_read_word((uint16_t*)2);
	unsigned short p = eeprom_read_word((uint16_t*)4);
	unsigned long cur = 1;
	unsigned short x = 0;
	while (1) {
		if (cur == b) {
			PORTA = (unsigned char)x;
			PORTB = (unsigned char)(x >> 8);
			break;
		}
		cur = (cur * a) % p;
		x++;
	}
	while (1) {
		continue;
	}
}
