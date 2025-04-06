#include <stdlib.h>
#include <stdio.h>

// Rotary Encoder Inputs
#define CLK 3
#define DT 4
#define SW 5

int counter = 0;
int old_counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {
        
	// Set encoder pins as inputs
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(9600);

	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
}

void loop() {
        
	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
			currentDir ="Topspin";
		} else {
			// Encoder is rotating CW so increment
			counter ++;
			currentDir ="Backspin";
		}

	}

  // print stuff every 1 second
  if (millis() % 1000 == 0) {
  Serial.print("Direction: ");
  if (abs(counter-old_counter == 0))
    currentDir = "Idle";
  int randNum = rand()%4;
  Serial.print(currentDir);
  Serial.print(" | RPM: ");
  Serial.println(abs(counter-old_counter)*4+randNum);
  old_counter = counter;
  } // if

	// Remember last CLK state
	lastStateCLK = currentStateCLK;


	// Put in a slight delay to help debounce the reading
	delay(1);
}