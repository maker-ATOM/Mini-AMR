/*
      EN	|		|	23  	--- 
ENLA	36	|		|	22	  SCL
ENLB	39	|		|	1 	  ---	
ENRA	34	|		|	3	    ---	
ENRB	35	|		|	21	  SDA	
LED2	32	|		|	19	  ---
LED3	33	|		|	18	  LED6
LED4	25	|		|	5	    ---	
LED1	26	|		|	17	  ---
LED5	27	|		|	16 	  MRB
MLP	  14	|		|	4 	  MRA
MLA	  12	|		|	2	    ---
MLB	  13	|		|	15	  MRP
    	GND	|		|	GND
	    VCC	|		|	3.3V


			      TOF2
			      LED2
TOF3	LED3				LED6	TOF1


TOF4	LED4				LED5	TOF6
		      	LED1
			      TOF5

 TOF Address 0x2_TOF_number

*/


// MPU6050 I2C address = 0x68

//-----------------------------------------------------------------------------------------------//

#include "MPU6050.h"
#include "Wire.h"
#include "Adafruit_VL53L0X.h"

//-----------------------------------------------------------------------------------------------//

#define MLP 14
#define MLA 12
#define MLB 13
#define MRP 15
#define MRA 4
#define MRB 16

byte mlpwm = 0;
byte mldir = 0;
byte mrpwm = 0;
byte mrdir = 0;

//-----------------------------------------------------------------------------------------------//

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

//-----------------------------------------------------------------------------------------------//

#define LOX1_ADDRESS 0x31
#define LOX2_ADDRESS 0x32
#define SHT_LOX1 2
#define SHT_LOX2 5

Adafruit_VL53L0X TOF_1 = Adafruit_VL53L0X();
Adafruit_VL53L0X TOF_2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

//-----------------------------------------------------------------------------------------------//

// #define LED_1 26
// #define LED_2 32
// #define LED_3 33
// #define LED_4 25
// #define LED_5 27
// #define LED_6 18

int led_pins[6] = { 26, 32, 33, 25, 27, 18 };
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

int ledpwm = 1;
int increment = 1;

//-----------------------------------------------------------------------------------------------//

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  TOF_1.begin(LOX1_ADDRESS);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  TOF_2.begin(LOX2_ADDRESS);
}

//-----------------------------------------------------------------------------------------------//

void setup() {

  Serial.begin(115200);

  ledcSetup(0, 5000, 8);
  for (int i = 0; i < 6; i++) {
    ledcAttachPin(led_pins[i], 0);
  }

  ledcSetup(1, 5000, 8);
  ledcAttachPin(MLP, 1);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(MRP, 2);
  pinMode(MLA, OUTPUT);
  pinMode(MLB, OUTPUT);
  pinMode(MRA, OUTPUT);
  pinMode(MRB, OUTPUT);

  ledcWrite(1, 255);
  digitalWrite(MLA, mldir);
  digitalWrite(MLB, !mldir);
  ledcWrite(2, 255);
  digitalWrite(MRA, mrdir);
  digitalWrite(MRB, !mrdir);

  Wire.begin();
  accelgyro.initialize();

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  setID();
}

//-----------------------------------------------------------------------------------------------//

void loop() {

  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  TOF_1.rangingTest(&measure1, false);
  TOF_2.rangingTest(&measure2, false);

  printData();

  currentMillis = millis();
  if (currentMillis - previousMillis >= 10) {
    ledPattern();
    previousMillis = currentMillis;
  }
}

//-----------------------------------------------------------------------------------------------//

void printData() {
  Serial.print("a/g:\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);
  Serial.print("\t");
  Serial.print(measure2.RangeMilliMeter);
  Serial.print("\t");
  Serial.println(measure1.RangeMilliMeter);
}

//-----------------------------------------------------------------------------------------------//

void ledPattern() {
  if (ledpwm > 120 || ledpwm < 1) {
    increment *= -1;
  }
  ledpwm += increment;

  ledcWrite(0, ledpwm);
}
