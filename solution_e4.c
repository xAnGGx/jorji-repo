#define FOSC 4000000UL
#define BAUD 115200 
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>

void UART_Init( unsigned int ubrr ){
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = 1 << RXEN0 | 1 << TXEN0;
    UCSR0C = 1 << USBS0 | 1 << UCSZ01 |  1 << UCSZ00;
    
    UBRR1H = (unsigned char)(ubrr>>8);
    UBRR1L = (unsigned char)ubrr;
    UCSR1B = 1 << RXEN1 | 1 << TXEN1;
    UCSR1C = 1 << USBS1 | 1 << UCSZ11 |  1 << UCSZ10;
}

void UART0_Transmit(unsigned char data ){
        while ( !(UCSR0A & (1 << UDRE0)) );
        UDR0 = data;
}

unsigned char UART0_Receive(){
        while ( !(UCSR0A & (1 << RXC0)) );
        return UDR0;
}

void UART1_Transmit(unsigned char data ){
        while ( !(UCSR1A & (1 << UDRE1)) );
        UDR1 = data;
}

unsigned char UART1_Receive(){
        while ( !(UCSR1A & (1 << RXC1)) );
        return UDR1;
}

int main() {
	unsigned char tmp;
	UART_Init(MYUBRR);
	while (!(UART0_Receive() & 0x02))
		UART1_Transmit(1);
	UART1_Transmit(2);
	while (1) {
		while (UART0_Receive() & 0x01) {
			UART1_Transmit(2);	// useless
			tmp = UART0_Receive();	// useless
			UART1_Transmit(0);	// useless
			while (UART0_Receive() & 0x02)
				UART1_Transmit(2);
			UART1_Transmit(1);
		}
		UART1_Transmit(0);
		tmp = UART0_Receive();	// useless
		UART1_Transmit(1);
	}
}
