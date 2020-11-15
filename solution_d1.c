#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
int main() {
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0xFF;
	PORTA = 0xFF;
	PORTB = 0xFF;
	unsigned char addr_a = PINA;
	unsigned char addr_b = PINB;
	unsigned char a = eeprom_read_byte((uint8_t*)addr_a);
	unsigned char b = eeprom_read_byte((uint8_t*)addr_b);
	PORTC = a;
	PORTC = b;
	while (1) {
		continue;
	}
}
