#include <avr/io.h>
#include <avr/eeprom.h>
int main() {
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0xFF;
	PORTA = 0xFF;
	PORTB = 0xFF;
	PORTC = eeprom_read_byte((uint8_t*)(uint16_t)PINA);
	PORTC = eeprom_read_byte((uint8_t*)(uint16_t)PINB);
	while (1) {
		continue;
	}
}
