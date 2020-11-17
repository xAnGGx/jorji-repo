#define FOSC 4000000UL
#define BAUD 115200 
#define MYUBRR FOSC/16/BAUD-1

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
	DDRA = 0xFF;
	DDRB = 0xFF;
	PORTA = 0xAA;
	PORTB = 0xAA;
	UART_Init();
	unsigned char cur;
	unsigned short cnt;
	while (1) {
		if (UART_Receive() == 'T')
}
