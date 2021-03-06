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
    unsigned char byte1, byte2, byte;
    while (1) {
    	byte1 = UART_Receive();
	byte2 = UART_Receive();
	byte = byte1 & byte2;
    	UART_Transmit(byte);
    }
}
