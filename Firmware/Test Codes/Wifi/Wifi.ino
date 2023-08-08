#define ROSSERIAL_ARDUINO_TCP
#include "WiFi.h"
#include <ros.h>
#include <geometry_msgs/Twist.h>

IPAddress server(192, 168, 1, 8);
uint16_t serverPort = 11411;
const char* ssid = "bsp74";
const char* password = "adianu09";


void cmdVel_to_pwm_cb(const geometry_msgs::Twist& velocity_msg);
ros::NodeHandle nh;
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &cmdVel_to_pwm_cb);

WiFiClient client;
void setup() {


  Serial.begin(115200);
  setupWiFi();
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.subscribe(sub);



  Serial.println("Get Ready");
  delay(2000);

  // Connect to ROS Serial Python node
  while (!client.connect(server, serverPort)) {
    Serial.println("Failed to connect to ROS Serial Python node. Retrying...");
    delay(1000);
  }
  Serial.println("Connected to ROS Serial Python node");
}




void loop() {

  nh.spinOnce();
}


void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP:   ");
  Serial.println(WiFi.localIP());
}



void cmdVel_to_pwm_cb(const geometry_msgs::Twist& velocity_msg) {

  Serial.println("we");
}
