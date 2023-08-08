#include <WiFi.h>
#include <ros.h>

IPAddress server(192, 168, 1, 8);
uint16_t serverPort = 11411;
const char* ssid = "bsp74";
const char* password = "adianu09";

WiFiClient client;
ros::NodeHandle nh;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to Wi-Fi. IP address: ");
  delay(1000);
  Serial.println(WiFi.localIP());

  // Set up ROS node
  nh.initNode();
  delay(1000);

  // Connect to ROS Serial Python node
  while (!client.connect(server, serverPort)) {
    Serial.println("Failed to connect to ROS Serial Python node. Retrying...");
    delay(1000);
  }
  Serial.println("Connected to ROS Serial Python node");
}

void loop() {
  // Handle ROS callbacks
  nh.spinOnce();

  // Publish messages or perform other operations
  // ...
}
