#include <SPI.h>
#include <Ethernet.h>
#include "DHT11.h"

#define pinTh  6 //Sensor de humedad y humedad.
#define pinLDR A4 //Sensor luminosidad.
DHT11 dht11(pinTh);

const int ledRed = 2; //Para cuando se active la alarma.
const int buzzer = 3;
const int door = 4;
const int ventilador = 5;
const int ledFoco = 8;
int valorLDR = 0;  

/* Variables para calcular la humedad*/
float te = 0;
float humedad = 0;

/* Variables necesarias para calcular la luminosidad */
const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ

int V;
int ilum;
/* Calculo para temperatura y caracteres personalizados */
float voltaje = 0;
float  temperatura = 0;

int doorValue = 1;
boolean isAlarmActive = false;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 17);

EthernetServer server(80);

void setup() {

  Serial.begin(9600);
  
  pinMode(ledRed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ventilador, OUTPUT);
  pinMode(ledFoco, OUTPUT);
  pinMode(pinLDR, INPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    String cadena = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (cadena.length()<50){
          cadena.concat(c);
           // Buscar campo data
          int posicion = cadena.indexOf("data");
          String command = cadena.substring(posicion);
 
          if (command == "data1=0"){
            digitalWrite(ventilador, HIGH);
          }else if (command == "data1=1"){
            digitalWrite(ventilador, LOW);
          }else if(command == "data1=2"){
            dht11.read(humedad, te);
            if(te>=18){
              digitalWrite(ventilador, HIGH);
              }else{
                digitalWrite(ventilador, LOW);
                }
            }else if (command == "data2=0"){
            isAlarmActive = true;
          }else if (command == "data2=1"){
            isAlarmActive = false;
            digitalWrite(buzzer, LOW);
            digitalWrite(ledRed, LOW);
          }else if(command == "data3=0"){
              V = analogRead(pinLDR);
              ilum = ((long)V * A * 10) / ((long)B * Rc * (1024 - V));
              if(ilum<1){
                  digitalWrite(ledFoco, HIGH);
              }else{
                digitalWrite(ledFoco, LOW);
              }
          }else if(command == "data3=1"){
              digitalWrite(ledFoco, LOW);
          }else if(command == "data3=2"){
              digitalWrite(ledFoco, HIGH);
          }
        }
        if(doorValue == 0 && isAlarmActive){
          digitalWrite(buzzer, HIGH);
          digitalWrite(ledRed, HIGH);
        }
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 3");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          //Realiza la lectura de humedad por medio de DHT11, es capaz de medir la temperatura
          dht11.read(humedad, te);
          doorValue = digitalRead(door);
          // Se realiza el calculo para la luminosidad con el foto resistor
          V = analogRead(pinLDR);
          ilum = ((long)V * A * 10) / ((long)B * Rc * (1024 - V));
         /* Creando la página web. */
          client.println(F("<html>\n<head>\n<title>Sistemas Programables</title>\n</head>\n<body style='background-color:#0B0B3B;'>"));
          client.println(F("<center>"));
          client.println(F("<p style='color:#FFFFFF; font-size:20px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial;'> Cuarto Inteligente </p>"));
          client.println(F("<p style='color:#e1f54f; font-size:20px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial;'> Sensores </p>"));
          client.println(F("<div style='color:#FFFFFF; font-size:13px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial;'>"));
          client.println("Temperatura: ");
          client.println(te);
          client.println(F("<br />"));
          client.println("Humedad: ");
          client.println(humedad);
          client.println(F("<br />"));
          client.println("Luminosidad: ");
          client.println(ilum);
          client.println(F("<br />"));
          client.print(F("Ventilador = "));
          client.println(digitalRead(ventilador) == LOW ? "OFF" : "ON");
          client.println(F("<br/>"));
          client.println(F("<button onClick=location.href='./?data1=0'>Activar</button>"));
          client.println(F("<button onClick=location.href='./?data1=1'>Desactivar</button>"));
          client.println(F("<button onClick=location.href='./?data1=2'>Automatico</button>"));
          client.println(F("<br/>"));
          client.print(F("Alarma = "));
          if(isAlarmActive){
            client.println("Activa");
          }else{client.println("Desactivada");}
          client.println(F("<br/>"));
          client.println(F("<button onClick=location.href='./?data2=0'>Activar</button>"));
          client.println(F("<button onClick=location.href='./?data2=1'>Desactivar</button>"));
          client.println(F("<br/>"));
          client.print(F("Luminocidad"));
          client.println(F("<br/>"));
          client.println(F("<button onClick=location.href='./?data3=0'>Automatico</button>"));
          client.println(F("<button onClick=location.href='./?data3=1'>Desactivar</button>"));
          client.println(F("<button onClick=location.href='./?data3=2'>Activar</button>"));
          client.println(F("<br/>"));
          client.println(F("</div>"));
          client.println(F("</center>\n</body></html>"));
          
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(50);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
