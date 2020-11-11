#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


const int     SMTP_PORT            = 465;
const char*   SMTP_SERVER          = "smtp.gmail.com";
String        error_message;
String        ServerResponse;
//################################################
const char*   ssid     = "Meher Bheree";                       // WIFI network name
const char*   password = "**********";                   // WIFI network password
//################################################
String Senders_Login    = "meherkiran.bheree@gmail.com";
String Senders_Password = "********"; // your email password
String From;
//################################################
String To, Subject, Message, Login_base64, Passwrd_base64;

WiFiClientSecure client;
  
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password); 
  Serial.print("Connecting to :"+String(ssid));
}

void loop() {
  To      = "meher.bheree@gmail.com";
  From    = "meherkiran.bheree@gmail.com";
  Subject = "Email from Arduino & ESP8266+";
  Message = "Hello World!!"; 
  SendMail(To, Subject, Message);
  if (error_message != "") Serial.println(error_message);
  delay(10000000);
}

void SendMail(String To, String Subject, String Message) {
  if (!client.connect(SMTP_SERVER, SMTP_PORT)) {
    error_message = "SMTP responded that it could not connect to the mail server";
    return;
  }  
  client.println("HELO server");
  client.println("AUTH LOGIN");
  WaitSMTPResponse(ServerResponse, 500);
  //client.println(base64::encode(Senders_Login));
  client.println("bWVoZXJraXJhbi5iaGVyZWVAZ21haWwuY29t");
  WaitSMTPResponse(ServerResponse, 500);
  client.println("TWVoZXJAMTk5Mw==");
  
  String mailFrom = "MAIL FROM: <" + String(From) + '>';
  client.println(mailFrom);
  WaitSMTPResponse(ServerResponse, 500);
  String recipient = "RCPT TO: <" + To + '>';
  client.println(recipient);
  WaitSMTPResponse(ServerResponse, 500);
  client.println("DATA");
  
  client.println("From: <" + String(From) + '>');
  client.println("To: <" + String(To) + '>');
  client.print("Subject: ");
  client.println(String(Subject));
  client.println("Mime-Version: 1.0");
  client.println("Content-Type: text/html; charset=\"UTF-8\"");
  client.println("Content-Transfer-Encoding: 7bit");
  client.println();
  String body = "<!DOCTYPE html><html lang=\"en\">" + Message + "</html>";
  client.println(body);
  client.println(".");
  
  client.println("QUIT");
 
  client.stop();
  Serial.println("Message Sent");
}

bool ErrorWhileWaitingForSMTP_Response(String Error_Code, int TimeOut) {
  int timer = millis();
  while (!client.available()) {
    if (millis() > (timer + TimeOut)) {
      error_message = "SMTP responsed that a Timeout occurred";
      return true;
    }
  }
  ServerResponse = client.readStringUntil('\n');
  if (ServerResponse.indexOf(Error_Code) == -1) return true;
  return false;
}

bool WaitSMTPResponse(String Error_Code, int TimeOut) {
  int timer = millis();
  while (!client.available()) {
    if (millis() > (timer + TimeOut)) {
      error_message = "SMTP responded that a Timeout occurred";
      return false;
    }
  }
  ServerResponse = client.readStringUntil('\n');
  if (ServerResponse.indexOf(Error_Code) == -1) return false;
  return true;
}
