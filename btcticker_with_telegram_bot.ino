#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
LiquidCrystal_I2C lcd(0x3F, 16, 2);
#include <UniversalTelegramBot.h>
#include <IFTTTMaker.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- WiFi Settings -------
char ssid[] = "UPC26D1381";       // your network SSID (name)
char password[] = "xxxxxxxx";  // your network key

#define TELEGRAM_BUTTON_PIN D5
int precio=0;


// ------- Telegram config --------
#define BOT_TOKEN "xxxxxx"  // your Bot Token (Get from Botfather)  434988650:BBHatx3QNbARboEkRggGCHcJbGH7nu-bb-b
#define CHAT_ID "xxxxxxx" // Chat ID of where you want the message to go (You can use MyIdBot to get the chat ID)   appplet id----60163523d

// SSL client needed for both libraries
WiFiClientSecure client;


UniversalTelegramBot bot(BOT_TOKEN, client);

String ipAddress = "";

volatile bool telegramButtonPressedFlag = false;
volatile bool iftttButtonPressedFlag = false;
volatile bool bitstampFlag = false;
volatile bool bitstampFlagerror = false;
char server[] = "www.bitstamp.net"; 

void setup()

{
  Serial.begin(115200);
    // Initlaze the buttons
  pinMode(TELEGRAM_BUTTON_PIN, INPUT);


  // NOTE:
  // It is important to use interupts when making network calls in your sketch
  // if you just checked the status of te button in the loop you might
  // miss the button press.
  attachInterrupt(TELEGRAM_BUTTON_PIN, telegramButtonPressed, RISING);


  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  ipAddress = ip.toString();


   delay(2000);
  Serial.println("connecting...");
}
void leer(){

//akiiiiiiiiiiiiiiiiiiiiiiiiii
  

//After that, we read the answer from the server:
String answer;
while(client.available()){
String line = client.readStringUntil('\r');
answer += line;
}
client.stop();
Serial.println();
Serial.println("closing connection");
delay(700);
String jsonAnswer;
int jsonIndex;
for (int i = 0; i < answer.length(); i++) {
  if (answer[i] == '{') {
    jsonIndex = i;
       break;
  }

}
//Then we extract the JSON object and place it in a string:
delay(500);
jsonAnswer = answer.substring(jsonIndex);
Serial.println();
Serial.println("JSON answer: ");
Serial.println(jsonAnswer);
jsonAnswer.trim();
//Once we have the JSON object we can get the Bitcoin price by doing some string operations:

int last = jsonAnswer.indexOf("last");
String priceString = jsonAnswer.substring(last + 8, last + 15);
priceString.trim();
String priceStringhigh = jsonAnswer.substring(last - 11, last - 4);
priceStringhigh.trim();
String priceStringvolume = jsonAnswer.substring(last + 93, last + 104);
priceStringvolume.trim();
float price = priceString.toFloat();
float pricehigh = priceStringhigh.toFloat();
float pricevolume = priceStringvolume.toFloat();
//We then print out the price on the serial monitor:

Serial.println();
Serial.println("Bitcoin price: ");
Serial.println(price);
Serial.println("Bitcoin high: ");
Serial.println(pricehigh);
Serial.println(pricevolume);

String message1 = " el precio del bitcoin es               ";
message1.concat(priceString);
message1.concat("\n el precio maximo es \n");
message1.concat(priceStringhigh);
message1.concat("\n el volumen es \n  ");
message1.concat(priceStringvolume);
bot.sendMessage(CHAT_ID, message1) ;

      lcd.init();   // initializing the LCD
      lcd.backlight(); // Enable or Turn On the backlight 
      lcd.setCursor(0, 0);
      lcd.print("precio bitcoin "); // Start Print text to Line 1
       lcd.setCursor(0, 1);      
       lcd.print(priceString); // Start Print Test to Line 2
       
  } 

// akiiiiiiiiiiiiiiiiiiii


void conectarbtc(){
 if (client.connect(server, 443)) { // tried port 443 - no answer at all
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /api/v2/ticker/btceur HTTP/1.1");
    client.println("Host: www.bitstamp.net");
    client.println("Connection: close");
    client.println();
    bitstampFlag=true;
  }
  }
void conectareth(){
 if (client.connect(server, 443)) { // tried port 443 - no answer at all
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /api/v2/ticker/etheur HTTP/1.1");
    client.println("Host: www.bitstamp.net");
    client.println("Connection: close");
    client.println();
    bitstampFlag=true;
  }
  }






void telegramButtonPressed() {
  Serial.println("telegramButtonPressed");
  int button = digitalRead(TELEGRAM_BUTTON_PIN);
  if(button == HIGH)
  {
    telegramButtonPressedFlag = true;
  }
  return;
}


void sendTelegramMessagebitstamp() {
  String message = "Conectado a bitstamp desde:  ";
  message.concat(ssid);
  message.concat("\n");
  message.concat("IP: ");
  message.concat(ipAddress);
  message.concat("\n");
  message.concat("a tradeaaaar \n");
   message.concat("\n");
    if(bot.sendMessage(CHAT_ID, message, "Markdown")){

}
bitstampFlag = false;
}
void sendTelegramMessagebitstamperror() {
  String message = "error conexion bitstamp desde:  ";
  message.concat(ssid);
  message.concat("\n");
  message.concat("IP: ");
  message.concat(ipAddress);
  message.concat("\n");
  message.concat("martillazo a la ardilla \n");
  if(bot.sendMessage(CHAT_ID, message, "Markdown")){
    Serial.println("TELEGRAM Successfully sent el error de conexion bitstamp");

}
bitstampFlagerror = false;
}
      
void sendTelegramMessage() {
  String message = "SSID:  ";
  message.concat(ssid);
  message.concat("\n");
  message.concat("IP: ");
  message.concat(ipAddress);
  message.concat("\n");
  if(bot.sendMessage(CHAT_ID, message, "Markdown")){
    Serial.println("TELEGRAM Successfully sent");
  }
  telegramButtonPressedFlag = false;
}
 
void loop()
{


  if ( bitstampFlagerror ) {
    sendTelegramMessagebitstamperror();
  } 
  if ( bitstampFlag ) {
    sendTelegramMessagebitstamp();
  } 
  if ( telegramButtonPressedFlag ) {
    sendTelegramMessage();
  } 

conectarbtc();
delay(1000);
leer();
delay(2000); 
 conectareth(); 
delay(1000);
leer();
 
delay(30000);
  
}
