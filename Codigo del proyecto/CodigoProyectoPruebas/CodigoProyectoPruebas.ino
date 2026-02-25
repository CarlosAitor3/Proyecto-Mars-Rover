// Definición de pines
const int MQ7_ANALOG_PIN = 34;  // Pin ADC para lectura analógica
const int MQ7_DIGITAL_PIN = 32; // Pin para lectura digital (umbral)

void setup() {
  Serial.begin(115200);
  pinMode(MQ7_DIGITAL_PIN, INPUT);
  
  Serial.println("--- Prueba de Sensor MQ-7 ---");
  Serial.println("Calentando el sensor (esto puede tomar un minuto para estabilizarse)...");
}

void loop() {
  // Lectura del valor analógico (0 - 4095 en ESP32)
  int analogValue = analogRead(MQ7_ANALOG_PIN);
  
  // Lectura del valor digital (0 o 1)
  int digitalValue = digitalRead(MQ7_DIGITAL_PIN);
  
  // Convertir a voltaje (referencia 3.3V en ESP32)
  float voltage = analogValue * (3.3 / 4095.0);

  // Mostrar resultados en el Monitor Serie
  Serial.print("Analógico: ");
  Serial.print(analogValue);
  Serial.print(" | Voltaje: ");
  Serial.print(voltage);
  Serial.print("V | Alerta Digital: ");
  Serial.println(digitalValue == LOW ? "DETECTADO" : "Limpio");

  delay(500); // Esperar medio segundo entre lecturas
}
