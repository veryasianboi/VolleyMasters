#define hall 3
int prev = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(hall, INPUT);
  Serial.println("setup");
}

void loop() {
  // put your main code here, to run repeatedly:
int bruh = digitalRead(hall);
//Serial.println(bruh);
if (bruh != prev) {
  Serial.println(bruh);
  prev = bruh;
} // if

}
