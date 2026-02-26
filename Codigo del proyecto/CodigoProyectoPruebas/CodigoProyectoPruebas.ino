#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <WiFi.h>
#include <ThingSpeak.h>

// --- CREDENCIALES WIFI ---
const char* ssid = "Aitor_Menta";
const char* password = "realmadrid";

WiFiClient cliente;

const char* apiKey = "6TF99KGWWULA6V2P"; //variables para enviar los datos al ThingSpeak
unsigned long minumerocanal = 3227073;

// Configuración DHT11
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configuración BMP180
Adafruit_BMP085 bmp;

// Configuración MQ7
const int MQ7_PIN = 34; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  // Inicia la conexión
  WiFi.begin(ssid, password);

  // Espera hasta que esté conectado
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Mensaje de éxito
  Serial.println("");
  Serial.println("¡WiFi conectado!");
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP());

  Serial.println("--- Iniciando Estación de Sensores 2026 ---");

  // Iniciar DHT11
  dht.begin();

  // Iniciar BMP180
  if (!bmp.begin()) {
    Serial.println("Error: No se encuentra el sensor BMP180. Verifica cables.");
    while (1) {}
  }

  // Configurar pin MQ7
  pinMode(MQ7_PIN, INPUT);
  
  Serial.println("Sensores listos.");

  ThingSpeak.begin(cliente); //comenzamos la comunicacion con ThingSpeak
}

void loop() {
  // --- Lectura DHT11 ---
  float h = dht.readHumidity();
  float t_dht = dht.readTemperature();

  // --- Lectura BMP180 ---
  float t_bmp = bmp.readTemperature();
  float p_bmp = bmp.readPressure(); // En Pascales
  float alt_bmp = bmp.readAltitude(101325); // Presión a nivel del mar estándar

  // --- Lectura MQ7 (Monóxido de Carbono) ---
  int mq7_raw = analogRead(MQ7_PIN);
  ThingSpeak.setField(1,mq7_raw); // ordinal es decir elegir campo, valor de la variable a guardar
  float mq7_voltage = mq7_raw * (3.3 / 4095.0);

  // --- Mostrar Datos en Monitor Serie ---
  Serial.println("=====================================");
  
  // Datos DHT11
  Serial.print("DHT11 -> Humedad: "); Serial.print(h); Serial.print("%  ");
  Serial.print("Temp: "); Serial.print(t_dht); Serial.println("°C");

  // Datos BMP180
  Serial.print("BMP180 -> Presión: "); Serial.print(p_bmp / 100.0); Serial.print(" hPa  ");
  Serial.print("Temp: "); Serial.print(t_bmp); Serial.print("°C  ");
  Serial.print("Altitud: "); Serial.print(alt_bmp); Serial.println(" m");

  // Datos MQ7
  Serial.print("MQ7 -> Valor Raw: "); Serial.print(mq7_raw); 
  ThingSpeak.writeFields(minumerocanal,apiKey); // comunicacion con ThingSpeak
  Serial.print(" | Voltaje: "); Serial.println(mq7_voltage);

  if (mq7_raw > 2000) { // Umbral de ejemplo
    Serial.println("¡ALERTA!: Concentración de gas elevada.");
  }

  delay(2000); // Pausa de 2 segundos entre lecturas

}

