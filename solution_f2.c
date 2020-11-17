#ifndef F_CPU
#define 4000000UL
#endif
void setup() {
	pinMode(3, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
}

void loop() {
	if (digitalRead(3)) {
		digitalWrite(0, HIGH);
		digitalWrite(1, LOW);
	} else {
		digitalWrite(0, LOW);
		digitalWrite(1, !digitalRead(5));
	}
}
