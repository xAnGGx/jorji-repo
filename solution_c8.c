#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
void send_cmd(unsigned char cmd) {
	PORTC &= 0b11111110;
	PORTA = cmd;
	PORTC |= 0b00000100;
	PORTC &= 0b11111011;
	_delay_us(50);
}

void send_data(unsigned char data) {
	PORTC |= 0b00000001;
	PORTA = data;
	PORTC |= 0b00000100;
	PORTC &= 0b11111011;
	_delay_us(50);
}

void lcd_init() {
	send_cmd(0b00001100);
	_delay_us(20);
	send_cmd(0b00000110);
	_delay_us(40);
}

int main() {
	DDRA = 0xFF;
	DDRC |= 0b00000111;
	PORTC &= 0b11111101;
	lcd_init();
	char* str = "Hello World!";
	for (int i = 0; str[i]; i++)
		send_data(str[i]);
	send_cmd(0b00100000);
}
