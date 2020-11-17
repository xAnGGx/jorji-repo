#ifndef F_CPU
#define 4000000UL
#endif
void setup() {
	pinMode(3, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(12, INPUT);
	digitalWrite(10, LOW);
}

void loop() {
	if (digitalRead(3)) {
		digitalWrite(0, HIGH);
		digitalWrite(1, LOW);
		digitalWrite(10, HIGH);
	}
	else {
		if (digitalRead(5)) {
			digitalWrite(0, LOW);
			digitalWrite(1, LOW);
			if (!digitalRead(12)) {
				digitalWrite(10, HIGH);
			}
			else {
				digitalWrite(10, LOW);
			}
		}
		else {
			digitalWrite(0, LOW);
			digitalWrite(1, HIGH);
			digitalWrite(10, LOW);
		}
	}
}
