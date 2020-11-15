#include <avr/io.h>
#include <utils/delay.h>
#include <avr/eeprom.h>
int main() {
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0x00;
	PORTA = 0xFF;
	PORTB = 0xFF;
	PORTC = 0xFF;
	eeprom_write_byte((uint8_t*)(uint16_t)PINA, PINC)
	eeprom_write_byte((uint8_t*)(uint16_t)PINB, PINC)
	_delay_ms(100);
	while (1) {
		continue;
	}
}
