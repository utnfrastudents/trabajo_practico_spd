#ifndef WIFIMODULE_INCLUDED
#define WIFIMODULE_INCLUDED
#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(3,2); // RX, TX del modulo WiFi ESP8266

/**
 	* Funcion que genera el comando AT para conectarse como cliente WiFi desde el modulo ESP8266
 	* stringSSID String Cadena del SSID del WiFi a conectarse.
 	* password String Cadena de tipo password del WiFi a conectarse.
 	* returns Cadena de conexion como Cliente WiFi.
 	* 
 	*/
String wifiString(String stringSSID, String password)
{
  String wifiCredentials = ""; /**< Cadena para armar el comando AT */
  
  /**< Se adiciona los segmentos del comando AT en funcion de los parametros ingresados */
  wifiCredentials += "AT+CWJAP=\"";
  wifiCredentials += stringSSID;
  wifiCredentials += "\",\"";
  wifiCredentials += password;
  wifiCredentials += "\"";

  return wifiCredentials;
}

/**
	* Funcion para enviar comandos AT via WiFi Serial al modulo ESP8266.
 	* commandAT String Comando AT a enviar.
 	* timeout const int Tiempo de espera de respuesta del modulo en milisegundos.
 	* debug boolean Se muestra el Debug en Serial Monitor? (true = si, false = no).
 	* returns Cadena de respuesta del modulo ESP8266.
 	* 
 	*/
String sendToWifi(String commandAT, const int timeout, boolean debug)
{
  String response = ""; /**< Cadena a retornar */
  long int time = millis(); /**< Tiempo actual en milisegundos */
  char character; /**< Caracter para almacenar la respuesta del modulo via WiFi Serial */

  wifiSerial.println(commandAT); /**< Se envia el comando AT al modulo ESP8266 */
  
  /**< Se obtiene la respuesta del modulo ESP8266 via WiFi Serial */
  while((time+timeout) > millis())
  {
    while(wifiSerial.available()>0)
    {
      wifiSerial.flush();
      
      /**< Si el modulo retorna valores de tipo caracter, se adiciona a la cadena de retorno de la funcion */
      character = wifiSerial.read(); /**< Se lee el caracter devuelto en cada pasada */
      response+=character;
    }
  }
  
  if(debug) /**< Si Debug es verdadero, mostrar la respuesta del modulo ESP8266 en Serial Monitor */
  {
    Serial.println(response);
  }
  
  return response;
}

/**
	*
	*/
boolean checkIP(const int timeout)
{
  boolean GET_IP = false;
  long int time = millis(); /**< Tiempo actual en milisegundos */
  char gotIP[] = "WIFI GOT IP";
  
  wifiSerial.flush();
  
  while((time+timeout) > millis())
  {
    while(wifiSerial.available()>0)
    {
      if(wifiSerial.find(gotIP))
      {
        GET_IP = true;
      }
    }
  }

  return GET_IP;
}

#endif
