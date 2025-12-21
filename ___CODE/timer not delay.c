//8Mhz xtal
int led;
int timer;

void setup() {
  pinMode (8, OUTPUT);
}

void loop() {
timer++;
digitalWrite(8, led);
if(timer%10000==0){led=!led;}
}

//16Mhz
int led;
int timer;

void setup() {
  pinMode (8, OUTPUT);
}

void loop() {
timer++;
digitalWrite(8, led);
if(timer%20000==0){led=!led;}
}

//MILLIS

unsigned long time;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  time = millis();

  Serial.println(time); //prints time since program started
  delay(1000);          // wait a second so as not to send massive amounts of data
}