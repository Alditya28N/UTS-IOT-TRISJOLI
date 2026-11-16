#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <time.h>

// WIFI
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT
const char* broker = "broker.hivemq.com";
const int port = 1883;

AsyncMqttClient mqtt;
String clientID;
String mqttTopic = "trisjoli/uts/iot/echo";

// SKENARIO S4
#define SKENARIO 4
#define QOS 0
#define PAYLOAD_SIZE 500
#define JUMLAH_PESAN 100
#define INTERVAL_MS 2000
#define TIMEOUT_MS 5000

bool mqttConnected = false;
bool mqttSubscribed = false;

volatile bool echoReceived = false;
volatile bool echoValid = false;
volatile bool waitingEcho = false;
volatile unsigned long echoTime = 0;

unsigned long waktuAwal;
bool rxValid = true;

// TIMESTAMP
String timestamp() {
  struct tm waktu;

  if (!getLocalTime(&waktu)) return "N/A";

  char buffer[25];
  strftime(buffer, sizeof(buffer),
           "%Y-%m-%d %H:%M:%S", &waktu);

  return String(buffer);
}

// WIFI
void koneksiWiFi() {
  Serial.println("Menghubungkan WiFi......");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// SINKRONISASI WAKTU
void sinkronisasiWaktu() {
  Serial.println("Sinkronisasi waktu");

  configTime(7 * 3600, 0,
             "pool.ntp.org", "time.google.com");

  struct tm waktu;

  while (!getLocalTime(&waktu)) {
    delay(500);
  }

  Serial.println("Waktu berhasil disinkronkan");
}

// CALLBACK MQTT
void onConnect(bool sessionPresent) {
  mqttConnected = true;
  mqtt.subscribe(mqttTopic.c_str(), 1);
}

void onDisconnect(AsyncMqttClientDisconnectReason reason) {
  mqttConnected = false;
  mqttSubscribed = false;
}

void onSubscribe(uint16_t packetId, uint8_t qos) {
  mqttSubscribed = true;
}

void onMessage(char* topic, char* data,
  AsyncMqttClientMessageProperties properties,
  size_t len, size_t index, size_t total) {

  if (strcmp(topic, mqttTopic.c_str()) != 0) return;
  if (!waitingEcho) return;

  if (index == 0) {
    rxValid = (total == PAYLOAD_SIZE);
  }

  for (size_t i = 0; i < len; i++) {
    if (data[i] != 'A') {
      rxValid = false;
    }
  }

  if (index + len >= total) {
    echoValid = rxValid;
    echoTime = millis();
    echoReceived = true;
  }
}

// KONEKSI MQTT
bool koneksiMQTT() {
  mqtt.onConnect(onConnect);
  mqtt.onDisconnect(onDisconnect);
  mqtt.onSubscribe(onSubscribe);
  mqtt.onMessage(onMessage);

  mqtt.setServer(broker, port);
  mqtt.setClientId(clientID.c_str());
  mqtt.setCleanSession(true);

  Serial.println("Menghubungkan MQTT......");
  mqtt.connect();

  unsigned long start = millis();

  while (!mqttConnected && millis() - start < 15000) {
    delay(10);
  }

  start = millis();

  while (mqttConnected && !mqttSubscribed &&
         millis() - start < 10000) {
    delay(10);
  }

  if (mqttConnected && mqttSubscribed) {
    Serial.println("MQTT Terhubung");
    Serial.print("Broker         : ");
    Serial.println(broker);
    Serial.print("Topic          : ");
    Serial.println(mqttTopic);
    return true;
  }

  return false;
}

// PENGUJIAN
void pengujianMQTT() {
  char payload[PAYLOAD_SIZE + 1];

  memset(payload, 'A', PAYLOAD_SIZE);
  payload[PAYLOAD_SIZE] = '\0';

  int berhasil = 0;
  int gagal = 0;

  waktuAwal = millis();

  Serial.println();
  Serial.println("==============================================================================================================");
  Serial.println("skenario seq    waktu_mulai         payload  t_kirim  t_terima  rtt   status  rssi");
  Serial.println("==============================================================================================================");

  for (int seq = 1; seq <= JUMLAH_PESAN; seq++) {

    if (!mqttConnected || !mqttSubscribed) {
      if (!koneksiMQTT()) {
        gagal++;
        delay(INTERVAL_MS);
        continue;
      }
    }

    echoReceived = false;
    echoValid = false;
    rxValid = true;
    waitingEcho = true;

    unsigned long waktuKirim = millis();
    String waktu = timestamp();

    // Publish QoS 0
    mqtt.publish(
      mqttTopic.c_str(),
      QOS,
      false,
      payload,
      PAYLOAD_SIZE
    );

    unsigned long tKirim = waktuKirim - waktuAwal;

    // Menunggu echo
    unsigned long start = millis();

    while (!echoReceived &&
           millis() - start < TIMEOUT_MS) {
      delay(1);
    }

    waitingEcho = false;

    if (echoReceived && echoValid) {
      unsigned long tTerima = echoTime - waktuAwal;
      unsigned long rtt = echoTime - waktuKirim;

      berhasil++;

      Serial.printf(
        "%-8d %-6d %-20s %-8d %-9lu %-10lu %-5lu %-8d %d\n",
        SKENARIO, seq, waktu.c_str(),
        PAYLOAD_SIZE, tKirim,
        tTerima, rtt, 1, WiFi.RSSI()
      );

    } else {
      gagal++;

      Serial.printf(
        "%-8d %-6d %-20s %-8d %-9lu %-10s %-5s %-8d %d\n",
        SKENARIO, seq, waktu.c_str(),
        PAYLOAD_SIZE, tKirim,
        "-", "-", 0, WiFi.RSSI()
      );
    }

    delay(INTERVAL_MS);
  }

  Serial.println("========================================");
  Serial.println("       HASIL PENGUJIAN MQTT");
  Serial.printf("Skenario       : %d\n", SKENARIO);
  Serial.printf("Protokol       : MQTT QoS %d\n", QOS);
  Serial.printf("Payload        : %d byte\n", PAYLOAD_SIZE);
  Serial.printf("Berhasil       : %d\n", berhasil);
  Serial.printf("Gagal          : %d\n", gagal);

  Serial.printf("PDR            : %.2f%%\n",
                berhasil * 100.0 / JUMLAH_PESAN);
}

// SETUP
void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("========================================");
  Serial.println("          PENGUJIAN MQTT");
  Serial.println("========================================");

  koneksiWiFi();
  sinkronisasiWaktu();

  uint64_t mac = ESP.getEfuseMac();

  char id[20];

  snprintf(id, sizeof(id), "%04X%08X",
           (uint16_t)(mac >> 32),
           (uint32_t)mac);

  clientID = "ESP32-" + String(id);

  if (!koneksiMQTT()) {
    Serial.println("Pengujian dibatalkan.");
    return;
  }

  Serial.println();
  Serial.println("========================================");
  Serial.println("          PENGUJIAN MQTT");
  Serial.println("========================================");

  Serial.printf("Skenario       : %d\n", SKENARIO);
  Serial.printf("Protokol       : MQTT QoS %d\n", QOS);
  Serial.printf("Jumlah pesan   : %d\n", JUMLAH_PESAN);
  Serial.printf("Payload        : %d byte\n", PAYLOAD_SIZE);
  Serial.printf("Interval       : %d ms\n", INTERVAL_MS);
  Serial.printf("Timeout        : %d ms\n", TIMEOUT_MS);
  Serial.printf("Waktu mulai    : %s\n", timestamp().c_str());

  pengujianMQTT();
}

void loop() {
  delay(1000);
}