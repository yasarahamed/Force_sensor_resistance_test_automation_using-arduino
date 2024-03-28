const int analogInPin = 15;  // Analog input
const int PIN_COUNT = 5;
const int SAMPLE_COUNT = 10;

const int pin[PIN_COUNT] = {17, 5, 18, 19, 22};
const float r1[PIN_COUNT] = {1000.0, 22000.0, 100000.0, 470000.0, 1000000.0};

const int numReadings = 5; // resistor smooth
float readings[numReadings];
int readIndex = 0;
float total = 0;
const int numSensor = 3; // no.of sensor connected
float average[numSensor]= {0};

//const int muxSIG = 12;
const int muxS3 = 14;
const int muxS2 = 27;
const int muxS1 = 26;
const int muxS0 = 25;
const int muxEN = 33;

int SetMuxChannel(byte channel)
{
  //Serial.print("F in");
  digitalWrite(muxS0, bitRead(channel, 0));
  digitalWrite(muxS1, bitRead(channel, 1));
  digitalWrite(muxS2, bitRead(channel, 2));
  digitalWrite(muxS3, bitRead(channel, 3));
  //Serial.print("F out");
  return channel;
}

void setup() {
  //pinMode(muxSIG, OUTPUT);
  pinMode(muxS0, OUTPUT);
  pinMode(muxS1, OUTPUT);
  pinMode(muxS2, OUTPUT);
  pinMode(muxS3, OUTPUT);
  pinMode(muxEN, OUTPUT);
  digitalWrite(muxEN, LOW);
  Serial.begin(115200);
  for (int i=0; i<PIN_COUNT; i++) {
     pinMode(pin[i], OUTPUT);
     digitalWrite(pin[i],LOW);
     pinMode(pin[i], INPUT);
  }
 // delay(100);
}

float averageVoltage() {
  float total = 0.0;
  for (int i=0;i<SAMPLE_COUNT;i++) {
   // delay(5);
    total += float(analogRead(analogInPin));
  }
  return 3.3*total/(SAMPLE_COUNT*4095.0);
}

float calculateResistance(float r, float v) {
   return r/(3.3/v-1.0);
}

void loop() {
   float minimum = 1.65;
   float resistance = 0; // default value
   for (byte j=0; j<numSensor; j++){
    int Fun = SetMuxChannel(j);
    //digitalWrite(muxSIG, LOW);
    //Serial.println("Mux high");
    for (int i=0; i<PIN_COUNT; i++) {
     pinMode(pin[i], OUTPUT);
     digitalWrite(pin[i],HIGH);
     float v = averageVoltage();
     digitalWrite(pin[i],LOW);
     pinMode(pin[i], INPUT);
     float difference = abs(v-1.65);
     if (3.3 > v && difference < minimum) {
        minimum = difference;
        resistance = calculateResistance(r1[i], v);
     }
    }
    total  = total - readings[readIndex];
    readings[readIndex] = resistance;
    total = total + readings[readIndex];
    readIndex = readIndex + 1;
    if (readIndex >= numReadings){
      readIndex =0;
    }
    average[j] = total / numReadings;
    Serial.print("Sensor ");
    Serial.print(j);
    Serial.print("is: ");
    Serial.print(average[j]);
    Serial.println(",");
//    delay(500);
//
   }
//   for(int k = 0; k < numSensor; k ++)
//   {
//      Serial.print(average[k]);
//      Serial.println(",");
//      delay(50);
//    }
  
//   Serial.print("resistance = ");
//   Serial.println(resistance);
//   delay(100);
//    Serial.print(average[0]);
//    Serial.print(",");
//    Serial.println(average[1]);
//    delay(50);
  
  

}
