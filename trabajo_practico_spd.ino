/** Sensor de Gas Metano con Alarma Sonora y Envio por Mail
 * 
 * Trabajo Practico de Sistema de Procesamiento de Datos
 * 
 * Participantes (Division: 1F):
 *    Alan Levy
 *    Gaston Pesoa
 *    Christian Grimberg 
 *    
 */

#include "wifiModule.h"
#define GAS_SENSE 460 /**< Limite de sensado del gas */
#define BUZZER 4 /**< Pin I/O Buzzer MH-FMD */
#define DELAY_SENSE 300 /**< Frecuencia de sensado */
#define DELAY_COMMAND 5000 /**< Tiempo de demora entre comandos */
#define INITIALIZE_DELAY 10000 /**< Tiempo de demora de inicializacion del Sensor MQ-9 */
#define RESPONSE_TIME 100 /**< Tiempo de respuesta de retorno de valores */
#define DEBUG true /**< Debug via Serial Monitor */
#define SSID_WIFI "TP SPD" /**< SSID WiFi a conectarse */
#define PASSWORD "martinSPD" /**< WiFi password en texto plano */
#define SMTP_SERVER "smtp.mail.com" /**< Servidor SMTP */

int gasSensor; /**< Para valor analogo del Sensor MQ-9 */

void setup()
{
  
  pinMode(BUZZER, OUTPUT); /**< Modo del PIN de Buzzer */
  
  digitalWrite(BUZZER, HIGH); /**< Comienza la salida en 1 para silenciar el Buzzer MH-FMD */

  /**< Serial Monitor Baudios */
  Serial.begin(9600);
  while (!Serial) {
    ; /**< Espera de conexion a Serial Port */
  }

  /**< Serial WiFi Baudios */
  wifiSerial.begin(115200);
  while (!wifiSerial) {
    ; /**< Espera de conexion a WiFi Serial Port */
  }
  
  Serial.println("============Connect to WiFi============");
  sendToWifi("AT+CWMODE=1", RESPONSE_TIME, DEBUG); /**< Configura el Modulo como Cliente WiFi */
  delay(DELAY_COMMAND);
    
  do
  {
    Serial.println("Connect to WiFi....");
    sendToWifi(wifiString(SSID_WIFI, PASSWORD), RESPONSE_TIME, DEBUG); /**< Se conecta a WiFi */
    delay(DELAY_COMMAND);
    //sendToWifi("AT+CIPSTATUS", RESPONSE_TIME, DEBUG); /**< WiFi Status */
    //sendToWifi("AT+CIFSR", RESPONSE_TIME, DEBUG); /**< WiFi info */
  } while(checkIP(RESPONSE_TIME));
  
  Serial.println("WiFi is connected!");
  Serial.println("=======================================");

  /**< Tiempo de demora de inicializacion del Sensor MQ-9 */
  Serial.println("\nWait until the sensor is initialized...\n");
  delay(INITIALIZE_DELAY);
}

void loop()
{
  gasSensor = analogRead(A0); /**< Lectura del valor analogo del Sensor MQ-9 */

  if(DEBUG) /**< Si Debug es verdadero, mostrar la lectura del Sensor MQ-9 en Serial Monitor */
  {
    Serial.println(gasSensor);
  }  

  if(gasSensor>GAS_SENSE)
  {
    digitalWrite(BUZZER, LOW); /**< Salida en 0 para hacer sonar el Buzzer MH-FMD */
    sendMail("Z2FzdG9ucGVzb2FAaG90bWFpbC5jb20=","eGVPWThzQWpncmFq", 2000, DEBUG);
  }    
  else
    digitalWrite(BUZZER, HIGH); /**< Salida en 1 para hacer sonar el Buzzer MH-FMD */

  delay(DELAY_SENSE); /**< Demora de la velocidad de frecuencia del sensado del MQ-9  */
}


