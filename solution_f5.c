#include <lcd_lib.h>
avrlib::LCD lcd(37, 35, AVR_PORTA);
void setup() {
	pinMode(35, OUTPUT);
	pinMode(37, OUTPUT);
	pinMode(22, OUTPUT);
	pinMode(23, OUTPUT);
	pinMode(24, OUTPUT);
	pinMode(25, OUTPUT);
	pinMode(26, OUTPUT);
	pinMode(27, OUTPUT);
	pinMode(28, OUTPUT);
	pinMode(29, OUTPUT);
	lcd.begin();
}

void loop() {
	lcd.print("Hello World!");
	lcd.setCursor(0, 0);
}
