void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  
}

long lastTime = 0;

void loop() {
  long total = 0;
  int num = 0;
    while(millis() - lastTime < 70)
    {
      total += (digitalRead(A0)) ? 255 : 0;
      num++;
    }

    lastTime = millis();

    Serial.println(((int)(total / num)));
}
