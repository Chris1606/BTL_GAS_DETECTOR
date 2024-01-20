#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

WebServer server(80);

#define GAS_SENSOR 15
#define GAS_SENSOR 4
#define BUZZER 19
#define LED 21
int GAS_VALUE;

void connectToWifi() {
  const char* ssid = "Ducthinh";
  const char* password = "20232024";
  WiFi.enableSTA(true);

  delay(2000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  String page = "<html>";
  page += "<head><script>function updateStatus() {";
  page += "var xhttp = new XMLHttpRequest();";
  page += "xhttp.onreadystatechange = function() {";
  page += "if (this.readyState == 4 && this.status == 200) {";
  page += "document.getElementById('status').innerHTML = this.responseText;";
  page += "}";
  page += "};";
  page += "xhttp.open('GET', '/status', true);";
  page += "xhttp.send();";
  page += "} setInterval(updateStatus, 1000);</script></head>";
  page += "<body><h1>GAS DETECTOR</h1>";
  page += "<div id='status'></div>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void handleStatus() {
  String status = (GAS_DETECT() == 1) ? "FIRE WARNING!!!!" : "NO FIRE WARNING";
  // Split the string into multiple lines
  status.replace("!!!!", "!!!!<br>");
  server.send(200, "text/html", status);
}

int GAS_DETECT() {
  GAS_VALUE = digitalRead(GAS_SENSOR);
  return (GAS_VALUE == HIGH) ? 1 : 0;
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  Serial.begin(115200);
  connectToWifi();
  beginServer();
  server.on("/status", HTTP_GET, handleStatus);
}

void loop() {
  GAS_DETECT();
  server.handleClient();
  if (GAS_DETECT()) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }
}
