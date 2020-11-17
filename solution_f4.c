#include "DHT.h"
#define DHTPIN 3
DHT dht(DHTPIN, DHT22);
void setup() {
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	dht.begin();
}

void loop() {
	delay(10);
	float h = dht.readHumidity();
	if (h < 45) {
		digitalWrite(0, HIGH);
		digitalWrite(1, LOW);
	}
	else {
		digitalWrite(0, LOW);
		if (h > 60) {
			digitalWrite(1, HIGH);
		}
		else {
			digitalWrite(1, LOW);
		}
	}
}
