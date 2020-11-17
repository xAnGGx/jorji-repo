#define FOSC 4000000UL
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>
void UART_Init( unsigned int ubrr ){
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = 1 << RXEN0 | 1 << TXEN0;
    UCSR0C = 1 << USBS0 | 1 << UCSZ01 |  1 << UCSZ00;
}

void UART_Transmit(unsigned char data ){
        while ( !(UCSR0A & (1 << UDRE0)) );
        UDR0 = data;
}

unsigned char UART_Receive(){
        while ( !(UCSR0A & (1 << RXC0)) );
        return UDR0;
}

int main() {
    UART_Init(MYUBRR);
    unsigned char byte;
    while (1) {
    	byte = UART_Receive();
	switch(byte) {
		case 0:
			UART_Transmit(62);
			UART_Transmit(90);
			UART_Transmit(101);
			UART_Transmit(114);
			UART_Transmit(111);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 1:
			UART_Transmit(62);
			UART_Transmit(79);
			UART_Transmit(110);
			UART_Transmit(101);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 2:
			UART_Transmit(62);
			UART_Transmit(84);
			UART_Transmit(119);
			UART_Transmit(111);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 3:
			UART_Transmit(62);
			UART_Transmit(84);
			UART_Transmit(104);
			UART_Transmit(114);
			UART_Transmit(101);
			UART_Transmit(101);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 4:
			UART_Transmit(62);
			UART_Transmit(70);
			UART_Transmit(111);
			UART_Transmit(117);
			UART_Transmit(114);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 5:
			UART_Transmit(62);
			UART_Transmit(70);
			UART_Transmit(105);
			UART_Transmit(118);
			UART_Transmit(101);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 6:
			UART_Transmit(62);
			UART_Transmit(83);
			UART_Transmit(105);
			UART_Transmit(120);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 7:
			UART_Transmit(62);
			UART_Transmit(83);
			UART_Transmit(101);
			UART_Transmit(118);
			UART_Transmit(101);
			UART_Transmit(110);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 8:
			UART_Transmit(62);
			UART_Transmit(69);
			UART_Transmit(105);
			UART_Transmit(103);
			UART_Transmit(104);
			UART_Transmit(116);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		case 9:
			UART_Transmit(62);
			UART_Transmit(78);
			UART_Transmit(105);
			UART_Transmit(110);
			UART_Transmit(101);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
		default:
			UART_Transmit(62);
			UART_Transmit(77);
			UART_Transmit(97);
			UART_Transmit(110);
			UART_Transmit(121);
			UART_Transmit(13);
			UART_Transmit(10);
			break;
	}
    }
}
