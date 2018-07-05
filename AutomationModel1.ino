#define FLOWSENSORPIN 2
#define SENSOR0PIN A0
#define SENSOR1PIN A1
#define SENSOR2PIN A2
#define SENSOR3PIN A3
#define SENSOR4PIN A4
#define SENSOR5PIN A5
#define SENSOR6PIN A6
#define SENSOR7PIN A7
#define SENSOR8PIN A8
#define SENSOR9PIN A9
#define SENSOR10PIN A10
#define SENSOR11PIN A11
#define SENSOR12PIN A12
#define SENSOR13PIN A13
#define SENSOR14PIN A14
#define TEMPSENSORPIN A15

int s0;
int s1;
int s2;
int s3;
int s4;
int s5;
int s6;
int s7;
int s8;
int s9;
int s10;
int s11;
int s12;
int s13;
int s14;
int stemp;

// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void setup() {
   Serial.begin(9600);
   Serial.println("MCU Readout:");
   
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
   useInterrupt(true);
}

void loop()                     // run over and over again
{ 
  // Flow Meter
  Serial.print("Freq: "); Serial.println(flowrate);
  Serial.print("Pulses: "); Serial.println(pulses, DEC);
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;
  Serial.print(liters); Serial.println(" Liters");

  //Sensor Read Outs
  s0 = analogRead(SENSOR0PIN);
  Serial.print("Sensor 0 readout: "); Serial.println(s0);
  s1 = analogRead(SENSOR1PIN);
  Serial.print("Sensor 1 readout: "); Serial.println(s1);
  s2 = analogRead(SENSOR2PIN);
  Serial.print("Sensor 2 readout: "); Serial.println(s2);
  s3 = analogRead(SENSOR3PIN);
  Serial.print("Sensor 3 readout: "); Serial.println(s3);
  s4 = analogRead(SENSOR4PIN);
  Serial.print("Sensor 4 readout: "); Serial.println(s4);
  s5 = analogRead(SENSOR5PIN);
  Serial.print("Sensor 5 readout: "); Serial.println(s5);
  s6 = analogRead(SENSOR6PIN);
  Serial.print("Sensor 6 readout: "); Serial.println(s6);
  s7 = analogRead(SENSOR7PIN);
  Serial.print("Sensor 7 readout: "); Serial.println(s7);
  
  delay(100);
}





