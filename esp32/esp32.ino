#include <WiFi.h>
#include <WebServer.h>


//#define WIFI_SSID "Jacob iPhone"
//#define WIFI_PASS "244466666"

#define WIFI_SSID "The Promised LAN"
#define WIFI_PASS "1a2b3c4d5e6X"

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Inicializa com serial

  Serial.print("Conectando a wifi");
  Serial.println("");
  WiFi.begin(WIFI_SSID, WIFI_PASS); //inicializar wifi
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conexion exitosa. IP: ");
  Serial.println(WiFi.localIP());

 //Especificamos las paginas que queremos para el codigo. Solo se necesita el principal y uno de 404
 server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("Servidor inicializado");
  
  delay(100);
}

 char parkingStatus = 0;

void loop() {
  server.handleClient();
  if (Serial.available() > 0) { 
  parkingStatus = Serial.read();
  Serial.println(parkingStatus, HEX);}
}



void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(parkingStatus));
}

String SendHTML(uint8_t parkingStatus) {
  String ptr = "<!DOCTYPE html> <html> <head> <title> Sistema de parqueo </title>\n";
ptr += "<style> html { background-color: #f2f9ee; } h1 { font-family: sans-serif; text-align: center; } .styled-table {background-color: #1d3556; border-collapse: collapse; margin: 25px 0; font-size: 0.9em; font-family: sans-serif; min-width: 400px; box-shadow: 0 0 20px rgba(0, 0, 0, 0.15); width: 100%; } .styled-table thead tr { background-color: tableMainColor; color: #ffffff; text-align: left;} .styled-table th, .styled-table td { padding: 12px 15px;} .styled-table tbody tr { border-bottom: 1px solid #dddddd;} .styled-table tbody tr:nth-of-type(even) {background-color: #f3f3f3;} .styled-table tbody tr:last-of-type {    border-bottom: 2px solid #1d3556;} .styled-table tbody tr.active-row {    font-weight: bold;    color: #009879;} .styled-table .ocupado { background-color: #e63947;} .styled-table .disponible {background-color: #a8d9dc;} .styled-table .izq {background-color: #f2f9ee;} </style> </head>\n";
ptr += "<body> <h1> Parqueos disponibles </h1> <table class = \"styled-table\"> <thead> <th> Espacio </th> <th> Disponibilidad </th> </thead> <tbody> \n";

if ((parkingStatus & 0x01) == 0x01) ptr += "<tr> <td class = \"izq\"> 1 </td> <td class = \"ocupado\"> Ocupado </td> </tr>\n";
else ptr += "<tr> <td class = \"izq\"> 1 </td> <td class = \"disponible\"> Libre </td> </tr>\n";

if ((parkingStatus & 0x02) == 0x02) ptr += "<tr> <td class = \"izq\"> 2 </td> <td class = \"ocupado\"> Ocupado </td> </tr>\n";
else ptr += "<tr> <td class = \"izq\"> 2 </td> <td class = \"disponible\"> Libre </td> </tr>\n";

if ((parkingStatus & 0x04) == 0x04) ptr += "<tr> <td class = \"izq\"> 3 </td> <td class = \"ocupado\"> Ocupado </td> </tr>\n";
else ptr += "<tr> <td class = \"izq\"> 3 </td> <td class = \"disponible\"> Libre </td> </tr>\n";

if ((parkingStatus & 0x08) == 0x08) ptr += "<tr> <td class = \"izq\"> 4 </td> <td class = \"ocupado\"> Ocupado </td> </tr>\n";
else ptr += "<tr> <td class = \"izq\"> 4 </td> <td class = \"disponible\"> Libre </td> </tr>\n";

ptr += "</tbody> </table> </body> </html>\n";
  return ptr;
}

void handle_NotFound() {
  server.send(404, "text/plain", "Error 404");
}
