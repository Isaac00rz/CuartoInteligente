/* Se importan las librerías a utilizar.
   | DHT11 - Sensor temperatura-humedad.
   | SPI, Client,Server, Ethernet - Nos ayudarán a
      utilizar la placa Ethernet W5100 a actuar como
      un servidor web.*/
#include "DHT11.h"
#include <SPI.h>
#include <Client.h>
#include <Ethernet.h>
#include <Server.h>

/* Pines para los sensores. */
#define pinTh  2 //Sensor de humedad y humedad.
#define pinLDR A4 //Sensor luminosidad.
DHT11 dht11(pinTh);

/*Pines para leds. */
const int ledRed = 7; //Para cuando se active la alarma.
const int ledWhite = 6; //Mal estado de temperatura.
const int ledGreen = 5; //Buen estado de temperatura.
/* Pin alarma y botón*/
const int buzzer = 4;
const int btn = 3;

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
byte celsius[8] = {
  0b11000,
  0b11011,
  0b00100,
  0b01000,
  0b01000,
  0b00100,
  0b00011,
  0b00000
};

/*Variables a utilizar para la placa Ethernet */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 10); //Debe ser una dirección que no se utilice.
EthernetServer server(80); //El puerto en el cual va a escuchar.
String rdString = String(30);

void setup()
{
  //Al inicializar, debemos levantar el servidor.
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(ledRed, OUTPUT);
  pinMode(ledWhite, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  Serial.begin(9600);
}

void loop()
{

  /*Sección 2. Escucha del servidor web */
  EthernetClient cliente = server.available();
  if (cliente)
  {
    boolean currentLineIsBlank = true;

    while (cliente.connected())
    {
      if (cliente.available())
      {
        char c = cliente.read();
        if (rdString.length() < 100) //Se lee peticiones de HTML caracter por caracter.
        {
          rdString += c;
        }

        if (c == "\n") //Si la petición se finaliza(del HTTP)..
        {
          //Aquí iria codigo bien shidori. Como encender los leds o no sé.
        }

        rdString = "";
        // Se envía la cabecera estándar de html.
        cliente.println("HTTP/1.1 200 OK");
        /* Por medio de println se envía la creación de la página web.*/
        // cliente.println("Connection: close"); // En caso de que no responda.
        // cliente.println("Refresh: 3"); // Refrescar la página automáticamente.
        cliente.println();
        //Realiza la lectura de humedad por medio de DHT11, es capaz de medir la temperatura
          dht11.read(humedad, te);
        
          // Se realiza el calculo para la luminosidad con el foto resistor
          V = analogRead(pinLDR);
          ilum = ((long)V * A * 10) / ((long)B * Rc * (1024 - V));
        /* Creando la página web. */
        cliente.println("<html>");
        cliente.println("<head>");
        cliente.println("<title> Sistemas Programables </title>");
        cliente.println("</head>");
        cliente.println("<body BGCOLOR=#0B0B3B>");//Cambiar color.
        cliente.println("<style> p { color:#FFFFFF; font-size:20px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial}</style>");
        cliente.println("<style> p1 { color:#e1f54f; font-size:20px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial}</style>");
        cliente.println("<style> p2 { color:#FFFFFF; font-size:13px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial}</style>");
        cliente.println("<center>");
        cliente.println("<p> Proyecto Final - Arduino IoT </p>");
        cliente.println("<hr><br>");
        cliente.println("<table>");
        cliente.println("<tr><td><p2>Alarma</p2></td><td>");
        cliente.println("<tr><td><form method=GET><input type=submit name=buzzer value=Encender></form></td>");
        cliente.println("<tr><td><form method=GET><input type=submit name=buzzer value=Apagar ></form>");
        cliente.println("</table>");
        cliente.println("<p1> Sensores </p1>");
        cliente.println("<hr><br>");
        cliente.println("<p2>");
        cliente.println("Temperatura: ");
        cliente.println(te);
        cliente.println("<br>");
        cliente.println("Humedad: ");
        cliente.println(humedad);
        cliente.println("<br>");
        cliente.println("Luminosidad: ");
        cliente.println(ilum);
        cliente.println("<br>");
        cliente.println("</p2>");
        cliente.println("</center>");
        cliente.println("</body>");
        cliente.println("</html>");
        cliente.stop(); 
      }
    }
  }

}
