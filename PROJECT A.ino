#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =====================================
// KONFIGURASI WIFI
// =====================================
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

// =====================================
// KONFIGURASI THINGSPEAK
// =====================================
unsigned long channelID = 1234567;
const char* writeAPIKey = "WRITE_API_KEY";

// =====================================
// KONFIGURASI DHT22
// =====================================
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// =====================================
// KONFIGURASI LCD I2C
// =====================================
#define SDA_PIN 21
#define SCL_PIN 22

LiquidCrystal_I2C lcd(0x27, 16, 2);

// =====================================
// WIFI CLIENT
// =====================================
WiFiClient client;

// =====================================
// INTERVAL PENGIRIMAN DATA
// =====================================
unsigned long previousMillis = 0;
const unsigned long interval = 30000;

// =====================================
// SETUP
// =====================================
void setup() {

  Serial.begin(115200);

  // Inisialisasi I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Inisialisasi sensor
  dht.begin();

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Monitoring IoT");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  delay(2000);

  // Menghubungkan WiFi
  WiFi.begin(ssid, password);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  Serial.print("Menghubungkan WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Terhubung!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  delay(1500);

  // Inisialisasi ThingSpeak
  ThingSpeak.begin(client);

  lcd.clear();
}

// =====================================
// LOOP
// =====================================
void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    // =================================
    // MEMBACA SENSOR DHT22
    // =================================

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // =================================
    // VALIDASI SENSOR
    // =================================

    if (isnan(temperature) || isnan(humidity)) {

      Serial.println("Gagal membaca sensor DHT22!");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor Error");

      return;
    }

    // =================================
    // MENAMPILKAN DATA SERIAL MONITOR
    // =================================

    Serial.println("==========================");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // =================================
    // MENAMPILKAN DATA LCD
    // =================================

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity, 1);
    lcd.print("%");

    // =================================
    // MENGIRIM DATA KE THINGSPEAK
    // =================================

    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (response == 200) {

      Serial.println("Data berhasil dikirim!");

    } else {

      Serial.print("Gagal mengirim data. HTTP Code: ");
      Serial.println(response);
    }

    Serial.println("==========================");
  }
}