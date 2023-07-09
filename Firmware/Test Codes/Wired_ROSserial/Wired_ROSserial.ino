#include <ros.h>
#include <geometry_msgs/Twist.h>


#define MRP 14
#define MRA 12
#define MRB 13
#define MLP 15
#define MLA 4
#define MLB 16

float mlpwm = 0;
byte mldir = 0;
float mrpwm = 0;
byte mrdir = 0;

ros::NodeHandle esp_node;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

int ledpwm = 1;
int increment = 1;

void vel_Callback(const geometry_msgs::Twist& vel_data) {
  mlpwm = vel_data.linear.x * 1000 - vel_data.angular.z * 1000;
  mrpwm = vel_data.linear.x * 1000 + vel_data.angular.z * 1000;

  mlpwm = map(mlpwm, -255, 255, -761, 761);  // 0.761 = max robot speed in m/s
  mrpwm = map(mrpwm, -255, 255, -761, 761);  // 0.761 = max robot speed in m/s

  if (mlpwm > 0) {
    mldir = 0;
  } else if (mlpwm < 0) {
    mldir = 1;
    mlpwm = -mlpwm;
  }

  if (mrpwm > 0) {
    mrdir = 0;
  } else if (mrpwm < 0) {
    mrdir = 1;
    mrpwm = -mrpwm;
  }

  mlpwm = constrain(mlpwm, 0, 255);
  mrpwm = constrain(mrpwm, 0, 255);
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &vel_Callback);

void setup() {
  pinMode(2, OUTPUT);
  esp_node.initNode();
  esp_node.subscribe(sub);

  ledcSetup(0, 5000, 8);
  ledcAttachPin(32, 0);
  ledcAttachPin(26, 0);


  ledcSetup(1, 5000, 8);
  ledcAttachPin(MLP, 1);
  ledcAttachPin(33, 1);
  ledcAttachPin(25, 1);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(MRP, 2);
  ledcAttachPin(18, 2);
  ledcAttachPin(27, 2);
  pinMode(MLA, OUTPUT);
  pinMode(MLB, OUTPUT);
  pinMode(MRA, OUTPUT);
  pinMode(MRB, OUTPUT);
}

void loop() {

  ledcWrite(1, mlpwm);

  digitalWrite(MLA, mldir);
  digitalWrite(MLB, !mldir);
  ledcWrite(2, mrpwm);
  digitalWrite(MRA, mrdir);
  digitalWrite(MRB, !mrdir);

  esp_node.spinOnce();
  currentMillis = millis();
  if (currentMillis - previousMillis >= 10) {
    ledPattern();
    previousMillis = currentMillis;
  }
}

void ledPattern() {
  if (ledpwm > 120 || ledpwm < 1) {
    increment *= -1;
  }
  ledpwm += increment;

  ledcWrite(0, ledpwm);
}
