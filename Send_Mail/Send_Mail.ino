/*
 *  HTTPS with follow-redirect example
 *  Created by Sujay S. Phadke, 2016
 *  
 *  Based on the WifiClientSecure example by
 *  Ivan Grokhotkov
 *  *
 *  This example is in public domain.
 *
 *  Modifed by PDAControl 2016 
 *  http://pdacontrolenglish.blogspot.com.co  
 *  http://pdacontrol.blogspot.com.co
 */

#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

// Replace with your network credentials
const char* ssid =     "1503523";
const char* password = "D2E7D32DBC883";

const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";
const char *GScriptId = "AKfycbzX2S1O4tJCkW7tP58jm4M1C"; /// GS Exe Google Script App

int cn = 0; 
const int httpsPort = 443;

// http://askubuntu.com/questions/156620/how-to-verify-the-ssl-fingerprint-by-command-line-wget-curl/
// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout
// www.grc.com doesn't seem to get the right fingerprint
// SHA1 fingerprint of the certificate
const char* fingerprint ="78:A4:31:E8:12:3E:50:79:3C:B3:10:80:B8:94:B1:36:23:B7:2E:9B";  ///// replace
//Tutorials
// http://pdacontrolenglish.blogspot.com.co/2016/06/esp8266-direct-connection-to-google.html
//http://pdacontrol.blogspot.com.co/2016/06/conexion-esp8266-directa-google.html

//video tutorial
// https://www.youtube.com/watch?v=5f7wCeD4gB4


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  // flush() is needed to print the above (connecting...) message reliably, 
  // in case the wireless connection doesn't go through
  Serial.flush();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create TLS connection
  HTTPSRedirect client(httpsPort);
  Serial.print("Connecting to ");
  Serial.println(host);

  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = client.connect(host, httpsPort);
    if (retval == 1) {
       flag = true;
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  
  Serial.flush();
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }
  
  Serial.flush();
  if (client.verify(fingerprint, host)) {
    Serial.println("Certificate match.");
  } else {
    Serial.println("Certificate mis-match");
  }

}

//funtion URL
String urlpacket( String GScriptId,String Address, String Message , String subject) {  
   
  //replace - remplazo  " " for "+" para envio atravez de url, for send via url  
   Message.replace(" ", "+");
   subject.replace(" ", "+");
  
   String url =  String("/macros/s/") + GScriptId + "/exec?"+ "Address=" + Address + "&Message=" + Message + "&subject="+ subject +  "&sendMail=ok";  
   return  url; 
 
  }


void loop() {
  HTTPSRedirect client(httpsPort);
  if (!client.connected())
    client.connect(host, httpsPort);
     Serial.println("run...");
    
     String Address ="jhonius22@gmail.com";   ///Email address
     
     String subject ="Test send Mail from PDAControl ";
     String Message ="Test send Mail from PDAControl Correct";
    
    
    if (cn==5){                                          //Counter = 10 send Mail  //// Contador = 10 envia correo

       Serial.println("hpta enviando mierda.......");
   //    Write to Google Spreadsheet     
     String urlout=  urlpacket(GScriptId,Address,Message,subject);
     client.printRedir(urlout, host, googleRedirHost);
     Serial.println("Mail enviado a .. "+ Address);
     Serial.println("===============================================================");
       Serial.println(urlout);

       cn=0;
    
        }
    
    cn++;
    Serial.println(cn);
    
  delay(1000);  //delay
}
