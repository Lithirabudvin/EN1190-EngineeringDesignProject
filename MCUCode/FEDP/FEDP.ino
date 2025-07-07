#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Preferences.h>
#include <time.h>
#include <ArduinoJson.h>

#include <WebServer.h>

const char *ssid_ap = "ESP32";
const char *password_ap = "12345678";


Preferences preferences;

WebServer server(80);  // Create a web server on port 80

String ssid_input;
String pass_input;

// HTML form to input SSID and Password
const char *html_form = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Wi-Fi Setup</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>Connect ESP32 to Wi-Fi</h2>
  <form action="/get">
    SSID: <input type="text" name="ssid"><br>
    Password: <input type="password" name="pass"><br><br>
    <input type="submit" value="Submit">
  </form>
</body>
</html>
)rawliteral";

// Function to handle the root page and send the form
void handleRoot() {
  server.send(200, "text/html", html_form);
}

void handleFormSubmit() {
  ssid_input = server.arg("ssid");
  pass_input = server.arg("pass");
  String response = "SSID: " + ssid_input + " | Password: " + pass_input;
  server.send(200, "text/plain", response);
  connectToWiFi(ssid_input.c_str(), pass_input.c_str());
  }


// Firebase project credentials
#define FIREBASE_HOST "kollo-5798c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "nuRRANasaLVujCDuIy5og7peYcyETOYblQYZRjeW"
// Network credentials

String node="IOT2";
String newssid =""; 
String newpassword =""; 

// Pins
const int switchPin1 = 27;
const int buttonPin1 = 33;
const int switchPin2 = 26;
const int buttonPin2 = 14;
const int switchPin3 = 17;
const int buttonPin3 = 16;
const int switchPin4 = 15;


const int currentSensorPin1 = 35;
const int currentSensorPin2 = 13;
const int currentSensorPin3 = 4;
const int voltageSensorPin = 25;

//ZMPT101B voltageSensor(25, 50.0);

//ZMPT101B voltageSensor(voltageSensorPin, 50.0);

bool switchState1 = LOW;
bool switchStae2 = LOW;
bool switchState3 = LOW;
bool switchState4 = LOW;
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;
//bool lastButtonState4 = HIGH;

// Firebase data
FirebaseData firebaseData;
FirebaseData stream1;
FirebaseData stream2;
FirebaseData stream3;
FirebaseData stream4;

FirebaseAuth auth;
FirebaseConfig config;

// Temperature data variables
int previousDay1 =0;
int previousDate1= 0;
int previousDay2 =0;
int previousDate2= 0;
int previousDay3 =4;
int previousDate3= 27;



float power1 = 0.0;
float power2 = 0.0;
float power3 = 0.0;

float max1=0;
float min1=0;
float rmsc1=0;
//float power1=0;


float max2=0;
float min2=0;
float rmsc2=0;
//float power2=0;

float max3=0;
float min3=0;
float rmsc3=0;
//float power3=0;

float sum1 = 0.0;
float sum2 = 0.0;
float sum3 = 0.0;

int firsttime = 0;

unsigned long previousMillis = 0;  // Stores the last time data was updated
const long interval = 6000; 


unsigned long lastUpdateTime1 = 0;
unsigned long lastUpdateTime2 = 0;
unsigned long lastUpdateTime3 = 0;

unsigned long updateInterval = 60000;


float Ta1 = 0.0, Tb1 = 0.0, Tc1 = 0.0, Td1 = 0.0, Te1 = 0.0, Tf1 = 0.0, Tg1 = 0.0, Th1 = 0.0;
float Ti1 = 0.0, Tj1 = 0.0, Tk1 = 0.0, Tl1 = 0.0, Tm1 = 0.0, Tn1 = 0.0, To1 = 0.0, Tp1 = 0.0;
float Tq1 = 0.0, Tr1 = 0.0, Ts1 = 0.0, Tt1 = 0.0, Tu1 = 0.0, Tv1 = 0.0, Tw1 = 0.0, Tx1 = 0.0;
float Ty1 = 0.0, Tz1 = 0.0;

float Ta2 = 0.0, Tb2 = 0.0, Tc2 = 0.0, Td2 = 0.0, Te2 = 0.0, Tf2 = 0.0, Tg2 = 0.0, Th2 = 0.0;
float Ti2 = 0.0, Tj2 = 0.0, Tk2 = 0.0, Tl2 = 0.0, Tm2 = 0.0, Tn2 = 0.0, To2 = 0.0, Tp2 = 0.0;
float Tq2 = 0.0, Tr2 = 0.0, Ts2 = 0.0, Tt2 = 0.0, Tu2 = 0.0, Tv2 = 0.0, Tw2 = 0.0, Tx2 = 0.0;
float Ty2 = 0.0, Tz2 = 0.0;

float Ta3 = 0.0, Tb3 = 0.0, Tc3 = 0.0, Td3 = 0.0, Te3 = 0.0, Tf3 = 0.0, Tg3 = 0.0, Th3 = 0.0;
float Ti3 = 0.0, Tj3 = 0.0, Tk3 = 0.0, Tl3 = 0.0, Tm3 = 0.0, Tn3 = 0.0, To3 = 0.0, Tp3 = 0.0;
float Tq3 = 0.0, Tr3 = 0.0, Ts3 = 0.0, Tt3 = 0.0, Tu3 = 0.0, Tv3 = 0.0, Tw3 = 0.0, Tx3 = 0.0;
float Ty3 = 0.0, Tz3 = 0.0;


float powers1[] = { Ta1, Tb1, Tc1, Td1, Te1, Tf1, Tg1, Th1, Ti1, Tj1, Tk1, Tl1, Tm1, Tn1, To1, Tp1, Tq1, Tr1, Ts1, Tt1, Tu1, Tw1, Tx1, Ty1, Tz1 };
float powers2[] = { Ta2, Tb2, Tc2, Td2, Te2, Tf2, Tg2, Th2, Ti2, Tj2, Tk2, Tl2, Tm2, Tn2, To2, Tp2, Tq2, Tr2, Ts2, Tt2, Tu2, Tw2, Tx2, Ty2, Tz2 };
float powers3[] = { Ta3, Tb3, Tc3, Td3, Te3, Tf3, Tg3, Th3, Ti3, Tj3, Tk3, Tl3, Tm3, Tn3, To3, Tp3, Tq3, Tr3, Ts3, Tt3, Tu3, Tw3, Tx3, Ty3 ,Tz3 };

const char *cpowers1[] = { "Ta1", "Tb1", "Tc1", "Td1", "Te1", "Tf1", "Tg1", "Th1", "Ti1", "Tj1", "Tk1", "Tl1", "Tm1", "Tn1", "To1", "Tp1", "Tq1", "Tr1", "Ts1", "Tt1", "Tu1", "Tw1", "Tx1", "Ty1", "Tz1" };
const char *cpowers2[] = { "Ta2", "Tb2", "Tc2", "Td2", "Te2", "Tf2", "Tg2", "Th2", "Ti2", "Tj2", "Tk2", "Tl2", "Tm2", "Tn2", "To2", "Tp2", "Tq2", "Tr2", "Ts2", "Tt2", "Tu2", "Tw2", "Tx2", "Ty2", "Tz2" };
const char *cpowers3[] = { "Ta3", "Tb3", "Tc3", "Td3", "Te3", "Tf3", "Tg3", "Th3", "Ti3", "Tj3", "Tk3", "Tl3", "Tm3", "Tn3", "To3", "Tp3", "Tq3", "Tr3", "Ts3", "Tt3", "Tu3", "Tw3", "Tx3", "Ty3", "Tz3" };

const char *firebase[] = { "Ta", "Tb", "Tc", "Td", "Te", "Tf", "Tg", "Th", "Ti", "Tj", "Tk", "Tl", "Tm", "Tn", "To", "Tp", "Tq", "Tr", "Ts", "Tt", "Tu", "Tw", "Tx", "Ty", "Tz" };




// Function to connect to WiFi
void connectToWiFi(const char* ssid, const char* password) {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to setup time using NTP
void setupTime() {
  configTime(19800, 0, "pool.ntp.org");
  Serial.println("Waiting for NTP time sync...");
  while (time(nullptr) < 100000) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nTime synchronized");
}


void updatePower1() {

  
  unsigned long currentMillis = millis();  // Get the current time


  float sensorValue = analogRead(currentSensorPin1);
  float current1 = pow((sensorValue / 1000.0), 2) * 1.26316 + (sensorValue/1000)*7.84669-17.3639;

  
  if (current1>max1){
    max1=current1;
    //latest_max=max1;
  }
   if (current1<min1){
    min1=current1;  
    //latest_min=min1;
  }

  if(current1<0.15 && current1 > -0.21 ){
  rmsc1 = 0;
  } 
  else{

  rmsc1=(max1-min1)/(2*pow((2), 0.5));
  }

  power1 = rmsc1 * 235 * 2.777778 * 1e-4;

  //;
  Serial.print("Sensor1:");
  Serial.println(current1);
  Serial.print("Sensor1max:");
  Serial.println(max1);
  Serial.print("Sensor1min:");
  Serial.println(min1);
  Serial.print("rms:");
  Serial.println(rmsc1);
  Serial.print("power1:");
  Serial.println(power1);


    if (currentMillis - previousMillis >= interval) {
    // Save the current time as the last update time
    previousMillis = currentMillis;
    min1=0;
    max1=0;
  }

}


void updatePower2() {

  
  unsigned long currentMillis = millis();  // Get the current time


  float sensorValue = analogRead(currentSensorPin2);
  float current2 = pow((sensorValue / 1000.0), 2) * 1.26316 + (sensorValue/1000)*7.84669-17.3639;

  
  if (current2>max2){
    max2=current2;
 
  }
   if (current2<min2){
    min2=current2;  
    //latest_min=min1;
  }

  if(current2<0.15 && current2 > -0.21 ){
  rmsc2 = 0;
  } 
  else{

  rmsc2=(max2-min2)/(2*pow((2), 0.5));
  }

  power2 = rmsc2 * 235 * 2.777778 * 1e-4;

  Serial.print("Sensor2:");
  Serial.println(sensorValue);
  Serial.print("Sensor2max:");
  Serial.println(max2);
  Serial.print("Sensor2min:");
  Serial.println(min2);
  Serial.print("rms:");
  Serial.println(rmsc2);
  Serial.print("power:");
  Serial.println(power2);


    if (currentMillis - previousMillis >= interval) {
    // Save the current time as the last update time
    previousMillis = currentMillis;
    min2=0;
    max2=0;
  }
   //temperature1 = (voltage-ACS712_OFFSET) / SENSITIVITY
}

void updatePower3() {

  unsigned long currentMillis = millis();  // Get the current time

  float sensorValue3 = analogRead(currentSensorPin3);
  float current3 = pow((sensorValue3 / 1000.0), 2) * 4.41282 + (sensorValue3/1000)*(-2.55047)-8.8362;

  
  if (current3>max3){
    max3=current3;
    //latest_max=max1;
  }
   if (current3<min3){
    min3=current3;  
    //latest_min=min1;
  }
    if(current3<0.15 && current3 > -0.21 ){
    rmsc3 = 0;
    } 
  else{

  rmsc3=(max3-min3)/(2*pow((2), 0.5));
       }

   power3 = rmsc3 * 235 * 2.777778 * 1e-4;

  Serial.print("Sensor3:");
  Serial.println(sensorValue3);
  Serial.print("Sensor3max:");
  Serial.println(max3);
  Serial.print("Sensor3min:");
  Serial.println(min3);
  Serial.print("rms:");
  Serial.println(rmsc3);
  Serial.print("power:");
  Serial.println(power3);

    if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    min3=0;
    max3=0;
  }

}



void fetchfirebase1() {
  for (int i = 0; i < 24; ++i) {
  if (Firebase.getFloat(firebaseData, ("/"+node+"/P1/"+firebase[i]))) {
    powers1[i] = firebaseData.floatData();
   }
  } 
  Serial.println("Firebase1");
}

void fetchfirebase2() {
  for (int i = 0; i < 24; ++i) {
  if (Firebase.getFloat(firebaseData, ("/"+node+"/P2/"+firebase[i]))) {
    powers2[i] = firebaseData.floatData();
   }
  } 
  Serial.println("Firebase2");
}

void fetchfirebase3() {
  for (int i = 0; i < 24; ++i) {
  if (Firebase.getFloat(firebaseData, ("/"+node+"/P3/"+firebase[i]))) {
    powers3[i] = firebaseData.floatData();
   }
  } 
  Serial.println("Firebase3");
}

void fetchdate1() {

  if (Firebase.getInt(firebaseData, ("/"+node+"/P1/Date/previousDate"))) {
    previousDate1 = firebaseData.intData();
    Serial.println("get succsess date1");
   }
}


void fetchdate2() {

  if (Firebase.getInt(firebaseData, ("/"+node+"/P2/Date/previousDate"))) {
    previousDate2 = firebaseData.intData();
    Serial.println("get succsess date2");
   }
}

void fetchdate3() {

  if (Firebase.getInt(firebaseData, ("/"+node+"/P3/Date/previousDate"))) {
    previousDate3 = firebaseData.intData();
    Serial.println("get succsess date3");
   }
}

void fetchday1() {

  if (Firebase.getInt(firebaseData, ("/"+node+"/P1/Date/previousDay"))) {
    previousDay1 = firebaseData.intData();
    Serial.println("get succsess day1");
   }
}

void fetchday2() {

  if (Firebase.getInt(firebaseData, ("/"+node+"/P2/Date/previousDay"))) {
    previousDay2 = firebaseData.intData();
    Serial.println("get succsess day2");
   }
}

void fetchday3() {

  if (Firebase.getInt(firebaseData, ("/"+node+"/P3/Date/previousDay"))) {
    previousDay3 = firebaseData.intData();
    Serial.println("get succsess day3");
   }
}

// Function to fetch the initial SWITCH state from Firebase
void fetchInitialswitchState1() {
  if (Firebase.getString(firebaseData, ("/"+node+"/P1/state/switchState"))) {
    String switchStateStr = firebaseData.stringData();
    switchState1 = (switchStateStr == "1") ? HIGH : LOW;
    digitalWrite(switchPin1
  , switchState1);
    Serial.println(switchState1 ? "SWITCH is ON" : "SWITCH is OFF");
  } else {
    Serial.println("Failed to fetch initial SWITCH state from Firebase1");
  }
}

void fetchInitialswitchState2() {
  if (Firebase.getString(firebaseData, ("/"+node+"/P2/state/switchState"))) {
    String switchStateStr = firebaseData.stringData();
    switchStae2 = (switchStateStr == "1") ? HIGH : LOW;
    digitalWrite(switchPin2, switchStae2);
    Serial.println(switchStae2 ? "SWITCH is ON" : "SWITCH is OFF");
  } else {
    Serial.println("Failed to fetch initial SWITCH state from Firebase2");
  }
}

void fetchInitialswitchState3() {
  if (Firebase.getString(firebaseData, ("/"+node+"/P3/state/switchState"))) {
    String switchStateStr = firebaseData.stringData();
    switchState3 = (switchStateStr == "1") ? HIGH : LOW;
    digitalWrite(switchPin3, switchState3);
    Serial.println(switchState3 ? "SWITCH is ON" : "SWITCH is OFF");
  } else {
    Serial.println("Failed to fetch initial SWITCH state from Firebase3");
  }
}

void fetchInitialswitchState4() {
  if (Firebase.getString(firebaseData, ("/"+node+"/P4/state/switchState"))) {
    String switchStateStr = firebaseData.stringData();
    switchState4 = (switchStateStr == "1") ? HIGH : LOW;
    digitalWrite(switchPin4, switchState4);
    Serial.println(switchState4 ? "SWITCH is ON" : "SWITCH is OFF");
  } else {
    Serial.println("Failed to fetch initial SWITCH state from Firebase4");
  }
}

// Function to send temperature data to Firebase
void sendPowerData0(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("Ta", powers[0]);
    json.set("Tb", powers[1]);
    json.set("Tc", powers[2]);
    json.set("Td", powers[3]);
    json.set("Te", powers[4]);
    json.set("Tf", powers[5]);
    json.set("Tg", powers[6]);
    json.set("Th", powers[7]);
    json.set("Ti", powers[8]);
    json.set("Tj", powers[9]);
    json.set("Tk", powers[10]);
    json.set("Tl", powers[11]);
    json.set("Tm", powers[12]);
    json.set("Tn", powers[13]);
    json.set("To", powers[14]);
    json.set("Tp", powers[15]);
    json.set("Tq", powers[16]);
    json.set("Tr", powers[17]);
    json.set("Ts", powers[18]);
    json.set("Tt", powers[19]);
    json.set("Tu", powers[20]);
    json.set("Tw", powers[21]);
    json.set("Tx", powers[22]);
    json.set("Ty", powers[23]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix, json)) {
        Serial.println("Firebase update successful");
    } else {
        Serial.println("Firebase update failed");
    }
}


void sendPowerData1(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("Mt", powers[24]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Total", json)) {
        Serial.println("Firebase update successful Mt" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed Mt" + nodeSuffix);
    }
}


void sendPowerData2(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("TUt", powers[24]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Total", json)) {
        Serial.println("Firebase update successful TUt" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed TUt" + nodeSuffix);
    }
}

void sendPowerData3(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("Wt", powers[24]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Total", json)) {
        Serial.println("Firebase update successful Wt" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed Wt" + nodeSuffix);
    }
}

void sendPowerData4(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("Tt", powers[24]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Total", json)) {
        Serial.println("Firebase update successful Tt" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed Tt" + nodeSuffix);
    }
}


void sendPowerData5(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("Ft", powers[24]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Total", json)) {
        Serial.println("Firebase update successful Ft" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed Ft" + nodeSuffix);
    }
}

void sendPowerData6(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("St", powers[24]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Total", json)) {
        Serial.println("Firebase update successful St" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed St" + nodeSuffix);
    }
}

void sendPowerData7(float powers[], const String& nodeSuffix) {
    FirebaseJson json;
    json.set("SUt", powers[24]);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Total", json)) {
        Serial.println("Firebase update successful SUt" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed SUt" + nodeSuffix);
    }
}

void senddate(int previousDate, const String& nodeSuffix) {
    FirebaseJson json;
    json.set("previousDate", previousDate);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Date", json)) {
        Serial.println("Firebase update successfuldate" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed date" + nodeSuffix);
    }
}

void sendday(int previousDay, const String& nodeSuffix) {
    FirebaseJson json;
    json.set("previousDay", previousDay);

    if (Firebase.updateNode(firebaseData, node + "/" + nodeSuffix+ "/Date", json)) {
        Serial.println("Firebase update successfulday" + nodeSuffix);
    } else {
        Serial.println("Firebase update failed day" + nodeSuffix);
    }
}


void setup() {

  Serial.begin(115200);
  preferences.begin("my-app", false);

  WiFi.softAP(ssid_ap, password_ap);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);  // Handle root URL
  server.on("/get", handleFormSubmit);  // Handle form submission
  server.begin();  // Start the server
  Serial.println("HTTP server started");

  

if (WiFi.status() == WL_CONNECTED) {
Serial.print("WIFIWorking");
  }

else{
  Serial.print("WifiNotWorking");

 }
//previousDate1 = 3;
//previousDay1 = 3;
//previousDate1 = preferences.getInt("previousDate1",0);
//previousDay1=preferences.getInt("previousDay1", 0);
//previousDate2 = preferences.getInt("previousDate2",0);
//previousDay2=preferences.getInt("previousDay2", 0);
//previousDate3 = preferences.getInt("previousDate3",0);
//previousDay3=preferences.getInt("previousDay3", 0);

}


void loop() {
  server.handleClient();

  if (WiFi.status() == WL_CONNECTED) {
    if (firsttime == 0){

  //preferences.begin("my-app", false);
  //voltageSensor.setSensitivity(SENSITIVITY);

  pinMode(switchPin1
, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  digitalWrite(switchPin1
, LOW);

  pinMode(switchPin2, OUTPUT);
  pinMode(buttonPin2, INPUT_PULLUP);
  digitalWrite(switchPin2, LOW);

  pinMode(switchPin3, OUTPUT);
  pinMode(buttonPin3, INPUT_PULLUP);
  digitalWrite(switchPin3, LOW);

  pinMode(switchPin4, OUTPUT);
  digitalWrite(switchPin4, LOW);

  //connectToWiFi(ssid,password);
  //setupTime();

  lastUpdateTime1 = millis();
  lastUpdateTime2 = millis();
  lastUpdateTime3 = millis();
  //lastUpdateTime4 = millis();



  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH; 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);



  fetchfirebase1();
  fetchfirebase2();
  fetchfirebase3();

  fetchdate1();
  fetchday1();

  fetchdate2();
  fetchday2();

  fetchdate3();
  fetchday3();

  fetchInitialswitchState1();
  fetchInitialswitchState2();
  fetchInitialswitchState3();
  fetchInitialswitchState4();

  if (!Firebase.beginStream(stream1, ("/"+node+"/P1/state"))) {
  Serial.printf("Stream begin error, %s\n\n", stream1.errorReason().c_str());
  }
    if (!Firebase.beginStream(stream2, ("/"+node+"/P2/state"))) {
  Serial.printf("Stream begin error, %s\n\n", stream2.errorReason().c_str());
  }
  if (!Firebase.beginStream(stream3, ("/"+node+"/P3/state"))) {
  Serial.printf("Stream begin error, %s\n\n", stream3.errorReason().c_str());
  }
  if (!Firebase.beginStream(stream4, ("/"+node+"/P4/state"))) {
  Serial.printf("Stream begin error, %s\n\n", stream4.errorReason().c_str());
  }

    }

  time_t now = time(nullptr);
  struct tm* timeInfo = localtime(&now);
  int currentSecond = timeInfo->tm_sec;
  int currentMinute = timeInfo->tm_min;
  int currentHour   = timeInfo->tm_hour;
  int currentDate   = timeInfo->tm_mday;
  int currentDay    = timeInfo->tm_wday;
  

  //  Serial.print("voltage:");
   // Serial.println(voltageSensor.getRmsVoltage());

    bool buttonState1 = digitalRead(buttonPin1);
    bool buttonState2 = digitalRead(buttonPin2);
    bool buttonState3 = digitalRead(buttonPin3);
    

    if (buttonState1 == LOW && lastButtonState1 == HIGH) {
      switchState1 = !switchState1;
      digitalWrite(switchPin1
    , switchState1);
      Serial.println(switchState1 ? "SWITCH is ON" : "SWITCH is OFF");

      FirebaseJson json;
      json.set("switchState", switchState1 ? "1" : "0");

      if (Firebase.updateNode(firebaseData, ("/"+node+"/P1/state"), json)) {
        Serial.println("Firebase SWITCH state updated");
      } else {
        Serial.println("Failed to update SWITCH state to Firebase");
      }
    }
    lastButtonState1 = buttonState1;

    if (!Firebase.readStream(stream1)) {
      Serial.printf("Stream read error, %s\n\n", stream1.errorReason().c_str());
    }

    if (stream1.streamTimeout()) {
      Serial.println("Stream timeout, resuming...\n");
      if (!Firebase.beginStream(stream1, ("/"+node+"/P1/state"))) {
        Serial.printf("Stream resume error, %s\n\n", stream1.errorReason().c_str());
      }
    }

    if (stream1.streamAvailable()) {
      Serial.printf("Stream data1 available\nEvent type: %s\nPath: %s\nData: %s\n\n", 
        stream1.eventType().c_str(), 
        stream1.dataPath().c_str(), 
        stream1.stringData().c_str());

      if (stream1.dataPath() == "/switchState") {
        int firebaseswitchState1 = atoi(stream1.stringData().c_str());
        if (firebaseswitchState1 != switchState1) {
          switchState1 = firebaseswitchState1;
          digitalWrite(switchPin1
        , switchState1 ? HIGH : LOW);
          Serial.println(switchState1 ? "SWITCH1 is ON (from Firebase)" : "SWITCH is OFF (from Firebase)");
        }
      }

      if (stream1.dataPath() == "/timestamp") {
        long timestamp = atol(stream1.stringData().c_str());
        Serial.printf("Timestamp received: %ld ms since start\n", timestamp);
      }
    }

     if (buttonState2 == LOW && lastButtonState2 == HIGH) {
      switchStae2 = !switchStae2;
      digitalWrite(switchPin2, switchStae2);
      Serial.println(switchStae2 ? "SWITCH is ON" : "SWITCH is OFF");

      FirebaseJson json;
      json.set("switchState", switchStae2 ? "1" : "0");

      if (Firebase.updateNode(firebaseData, ("/"+node+"/P2/state"), json)) {
        Serial.println("Firebase SWITCH state updated");
      } else {
        Serial.println("Failed to update SWITCH state to Firebase");
      }
    }
    lastButtonState2 = buttonState2;

    if (!Firebase.readStream(stream2)) {
      Serial.printf("Stream read error, %s\n\n", stream2.errorReason().c_str());
    }

    if (stream2.streamTimeout()) {
      Serial.println("Stream timeout, resuming...\n");
      if (!Firebase.beginStream(stream2, ("/"+node+"/P2/state"))) {
        Serial.printf("Stream resume error, %s\n\n", stream2.errorReason().c_str());
      }
    }

    if (stream2.streamAvailable()) {
      Serial.printf("Stream data2 available\nEvent type: %s\nPath: %s\nData: %s\n\n", 
        stream2.eventType().c_str(), 
        stream2.dataPath().c_str(), 
        stream2.stringData().c_str());

      if (stream2.dataPath() == "/switchState") {
        int firebaseswitchState2 = atoi(stream2.stringData().c_str());
        if (firebaseswitchState2 != switchStae2) {
          switchStae2 = firebaseswitchState2;
          digitalWrite(switchPin2, switchStae2 ? HIGH : LOW);
          Serial.println(switchStae2 ? "SWITCH is ON (from Firebase)" : "SWITCH is OFF (from Firebase)");
        }
      }

      if (stream2.dataPath() == "/timestamp") {
        long timestamp = atol(stream2.stringData().c_str());
        Serial.printf("Timestamp received: %ld ms since start\n", timestamp);
      }
    }



    if (buttonState3 == LOW && lastButtonState3 == HIGH) {
    switchState3 = !switchState3;
    digitalWrite(switchPin3, switchState3);
    Serial.println(switchState3 ? "IOT3 SWITCH is ON" : "IOT3 SWITCH is OFF");

    FirebaseJson json;
    json.set("switchState", switchState3 ? "1" : "0");

    if (Firebase.updateNode(firebaseData, ("/" + node + "/P3/state"), json)) {
      Serial.println("IOT3 Firebase SWITCH state updated");
    } else {
      Serial.println("Failed to update IOT3 SWITCH state to Firebase");
    }
  }
  lastButtonState3 = buttonState3;

  // Stream handling for IOT3
  if (!Firebase.readStream(stream3)) {
    Serial.printf("IOT3 Stream read error, %s\n\n", stream3.errorReason().c_str());
  }

  if (stream3.streamTimeout()) {
    Serial.println("IOT3 Stream timeout, resuming...\n");
    if (!Firebase.beginStream(stream3, ("/" + node + "/P3/state"))) {
      Serial.printf("IOT3 Stream resume error, %s\n\n", stream3.errorReason().c_str());
    }
  }

  if (stream3.streamAvailable()) {
    Serial.printf("IOT3 Stream data available\nEvent type: %s\nPath: %s\nData: %s\n\n",
      stream3.eventType().c_str(),
      stream3.dataPath().c_str(),
      stream3.stringData().c_str());

    if (stream3.dataPath() == "/switchState") {
      int firebaseswitchState3 = atoi(stream3.stringData().c_str());
      if (firebaseswitchState3 != switchState3) {
        switchState3 = firebaseswitchState3;
        digitalWrite(switchPin3, switchState3 ? HIGH : LOW);
        Serial.println(switchState3 ? "IOT3 SWITCH is ON (from Firebase)" : "IOT3 SWITCH is OFF (from Firebase)");
      }
    }

    if (stream3.dataPath() == "/timestamp") {
      long timestamp = atol(stream3.stringData().c_str());
      Serial.printf("IOT3 Timestamp received: %ld ms since start\n", timestamp);
    }
  }


  // Stream handling for IOT4
  if (!Firebase.readStream(stream4)) {
    Serial.printf("IOT4 Stream read error, %s\n\n", stream4.errorReason().c_str());
  }

  if (stream4.streamTimeout()) {
    Serial.println("IOT4 Stream timeout, resuming...\n");
    if (!Firebase.beginStream(stream4, ("/" + node + "/P4/state"))) {
      Serial.printf("IOT4 Stream resume error, %s\n\n", stream4.errorReason().c_str());
    }
  }

  if (stream4.streamAvailable()) {
    Serial.printf("IOT4 Stream data available\nEvent type: %s\nPath: %s\nData: %s\n\n",
      stream4.eventType().c_str(),
      stream4.dataPath().c_str(),
      stream4.stringData().c_str());

    if (stream4.dataPath() == "/switchState") {
      int firebaseswitchState4 = atoi(stream4.stringData().c_str());
      if (firebaseswitchState4 != switchState4) {
        switchState4 = firebaseswitchState4;
        digitalWrite(switchPin4, switchState4 ? HIGH : LOW);
        Serial.println(switchState4 ? "IOT4 SWITCH is ON (from Firebase)" : "P4 SWITCH is OFF (from Firebase)");
      }
    }

    if (stream4.dataPath() == "/timestamp") {
      long timestamp = atol(stream4.stringData().c_str());
      Serial.printf("IOT4 Timestamp received: %ld ms since start\n", timestamp);
    }
  }
  
  if(switchState1==1){

  updatePower1();


  if (millis() - lastUpdateTime1 >= updateInterval) {
    sendPowerData0(powers1, "P1");
    lastUpdateTime1 = millis();

    if (currentDay==1){
    sendPowerData1(powers1, "P1");
    } 
    if (currentDay==2){
    sendPowerData2(powers1, "P1");
    }
    if (currentDay==3){
    sendPowerData3(powers1, "P1");
    }
    if (currentDay==4){
    sendPowerData4(powers1, "P1");
    }
    if (currentDay==5){
    sendPowerData5(powers1, "P1");
    }
    if (currentDay==6){
    sendPowerData6(powers1, "P1");
    }
     if (currentDay==0){
    sendPowerData7(powers1, "P1");
    }
  }


// Update temperature data based on current hour
for (int i = 0; i < 24; ++i) {

  if (currentHour == i) powers1[i] += power1;
  sum1 += powers1[i] ;
 // preferences.putFloat(ctemperatures1[i], temperatures1[i]);
    }


   powers1[24] = sum1;
   sum1 = 0;

    // Store updated values in preferences
//for (int i = 0; i < 24; ++i) {
  
    //preferences.putFloat(ctemperatures1[i], temperatures1[i]);
    //}



    if ((currentHour == 23 && currentMinute == 59 && currentSecond == 59) || (currentDate != previousDate1) || (currentDay != previousDay1) ) {
    Serial.println("KKK");

    for (int i = 0; i < 25; ++i) {
  
      powers1[i] = 0 ;
      //preferences.putFloat(ctemperatures1[i] , temperatures1[i]);
      sendPowerData0(powers1, "P1");
      }
    }


  if (currentDate != previousDate1){
    previousDate1=currentDate;
    senddate(previousDate1,"P1");
    Serial.print(previousDate1);
    }


  if (previousDay1 != currentDay){
    previousDay1=currentDay;
    sendday(previousDay1,"P1");
    Serial.print(previousDay1);
    }

}


  if(switchStae2==1){

  updatePower2();


  if (millis() - lastUpdateTime2 >= updateInterval) {
    sendPowerData0(powers2, "P2");
    lastUpdateTime2 = millis();

    if (currentDay==1){
    sendPowerData1(powers2, "P2");
    } 
    if (currentDay==2){
    sendPowerData2(powers2, "P2");
    }
    if (currentDay==3){
    sendPowerData3(powers2, "P2");
    }
    if (currentDay==4){
    sendPowerData4(powers2, "P2");
    }
    if (currentDay==5){
    sendPowerData5(powers2, "P2");
    }
    if (currentDay==6){
    sendPowerData6(powers2, "P2");
    }
     if (currentDay==0){
    sendPowerData7(powers2, "P2");
    }
  }


// Update temperature data based on current minute

    // Store updated values in preferences
for (int i = 0; i < 24; ++i) {

  if (currentHour == i) powers2[i] += power2;
  sum2 += powers2[i] ;
  //preferences.putFloat(ctemperatures2[i], temperatures2[i]);
    }


   powers2[24] = sum2;
   sum2 = 0;

    if ((currentHour == 23 && currentMinute == 59 && currentSecond == 59) || (currentDate != previousDate2) || (currentDay != previousDay2) ) {
    Serial.println("KKK");

    // Reset temperature accumulation
    for (int i = 0; i < 25; ++i) {
  
      powers2[i] = 0 ;
      //preferences.putFloat(ctemperatures2[i] , temperatures2[i]);
    }
        sendPowerData0(powers2, "P2");
    }
  

   if (currentDate != previousDate2){
    previousDate2=currentDate;
    senddate(previousDate2,"P2");
    Serial.print(previousDate2);
    }

  if (previousDay2 != currentDay){
    previousDay2=currentDay;
    sendday(previousDay2,"P2");
    Serial.print(previousDay2);
    }
 
  }


    
  if(switchState3==1){

  updatePower3();


  if (millis() - lastUpdateTime3 >= updateInterval) {
    sendPowerData0(powers3, "P3");
    lastUpdateTime3 = millis();

    if (currentDay==1){
    sendPowerData1(powers3, "P3");
    } 
    if (currentDay==2){
    sendPowerData2(powers3, "P3");
    }
    if (currentDay==3){
    sendPowerData3(powers3, "P3");
    }
    if (currentDay==4){
    sendPowerData4(powers3, "P3");
    }
    if (currentDay==5){
    sendPowerData5(powers3, "P3");
    }
    if (currentDay==6){
    sendPowerData6(powers3, "P3");
    }
     if (currentDay==0){
    sendPowerData7(powers3, "P3");
    }
  }


// Update temperature data based on current minute
for (int i = 0; i < 24; ++i) {

  if (currentHour == i) powers3[i] += power3;
  sum3 += powers3[i] ;
  //preferences.putFloat(ctemperatures3[i], temperatures3[i]);
    }

   powers3[24] = sum3;
   sum3 = 0;

    if ((currentHour == 23 && currentMinute == 59 && currentSecond == 59) || (currentDate != previousDate3) || (currentDay != previousDay3) ) {
    Serial.println("KKK");

    // Reset temperature accumulation
    for (int i = 0; i < 25; ++i) {
  
      powers3[i] = 0 ;
      //preferences.putFloat(ctemperatures3[i] , temperatures3[i]);
    }
      sendPowerData0(powers3, "P3");
    }
  

  if (currentDate != previousDate3){
    previousDate3=currentDate;
    senddate(previousDate3,"P3");
    Serial.print(previousDate3);
    }

  if (previousDay3 != currentDay){
    previousDay3=currentDay;
    sendday(previousDay3,"P3");
    Serial.print(previousDay3);
    }


 }
 
 firsttime = 1;

  delay(500); 
  }
  else{
    firsttime = 0;
    Serial.print("wifidisconnected");
  }
}
