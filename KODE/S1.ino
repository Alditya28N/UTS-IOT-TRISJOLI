#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

// ================= WIFI =================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ================= HTTP =================
const char* serverURL = "http://httpbin.org/post";

// ================= PARAMETER PENGUJIAN =================
const int SCENARIO = 1;
const int TOTAL_PESAN = 100;
const unsigned long INTERVAL = 2000;
const unsigned long TIMEOUT = 5000;
const int PAYLOAD_SIZE = 20;

// ================= VARIABEL =================
unsigned long waktuMulai;
unsigned long waktuSelesai;

int berhasil = 0;
int gagal = 0;

// ================= FORMAT WAKTU =================
String getCurrentTime() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    return "Waktu tidak tersedia";
  }

  char buffer[25];

  strftime(buffer, sizeof(buffer),
           "%Y-%m-%d %H:%M:%S", &timeinfo);

  return String(buffer);
}

// ================= WIFI =================
void connectWiFi() {
  Serial.print("Menghubungkan WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// ================= NTP WIB =================
void setupTime() {
  configTime(7 * 3600, 0, "pool.ntp.org");

  Serial.print("Sinkronisasi waktu");

  struct tm timeinfo;

  while (!getLocalTime(&timeinfo)) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWaktu berhasil disinkronkan");
}

// ================= FORMAT TABEL =================
void printHeader() {
  Serial.println();
  Serial.println("========================================================================================================");
  Serial.println("skenario seq    waktu_mulai          payload  t_kirim  t_terima  rtt  status  rssi");
  Serial.println("========================================================================================================");
}

// ================= PENGUJIAN HTTP =================
void runScenario() {

  HTTPClient http;

  berhasil = 0;
  gagal = 0;

  waktuMulai = millis();

  String waktuMulaiPengujian = getCurrentTime();

  Serial.println("\n========================================");
  Serial.println("       PENGUJIAN HTTP KEEP-ALIVE");
  Serial.println("========================================");

  Serial.print("Skenario       : ");
  Serial.println(SCENARIO);

  Serial.print("Jumlah pesan   : ");
  Serial.println(TOTAL_PESAN);

  Serial.print("Payload        : ");
  Serial.print(PAYLOAD_SIZE);
  Serial.println(" byte");

  Serial.print("Content-Length : ");
  Serial.println(PAYLOAD_SIZE);

  Serial.print("Interval       : ");
  Serial.print(INTERVAL);
  Serial.println(" ms");

  Serial.print("Timeout        : ");
  Serial.print(TIMEOUT);
  Serial.println(" ms");

  Serial.print("Waktu mulai    : ");
  Serial.println(waktuMulaiPengujian);

  // ================= HTTP BEGIN =================
  http.begin(serverURL);

  http.setReuse(true);
  http.setConnectTimeout(TIMEOUT);
  http.setTimeout(TIMEOUT);

  http.addHeader("Content-Type", "text/plain");
  http.addHeader("Connection", "keep-alive");

  printHeader();

  // ================= LOOP PENGUJIAN =================
  for (int seq = 1; seq <= TOTAL_PESAN; seq++) {

    // Menentukan waktu pengiriman
    unsigned long targetTime =
      waktuMulai + ((unsigned long)(seq - 1) * INTERVAL);

    while ((long)(millis() - targetTime) < 0) {
      delay(1);
    }

    // ================= WAKTU MULAI =================
    String waktuMulaiPesan = getCurrentTime();

    unsigned long tKirim = millis() - waktuMulai;

    // ================= PAYLOAD 20 BYTE =================
    String body = "";

    for (int i = 0; i < PAYLOAD_SIZE; i++) {
      body += "A";
    }

    int contentLength = body.length();

    // ================= HTTP REQUEST =================
    int httpCode = -1;

    unsigned long tTerima = 0;
    unsigned long rtt = 0;

    String terimaStr = "-";
    String rttStr = "-";

    // ================= PENGIRIMAN =================
    if (WiFi.status() == WL_CONNECTED) {

      unsigned long requestStart = millis();

      httpCode = http.POST(
        (uint8_t*)body.c_str(),
        contentLength
      );

      // Membaca seluruh respons server
      String response = "";

      if (httpCode > 0) {
        response = http.getString();
      }

      unsigned long requestEnd = millis();

      rtt = requestEnd - requestStart;

      if (httpCode >= 200 && httpCode < 300) {

        tTerima = requestEnd - waktuMulai;

        terimaStr = String(tTerima);
        rttStr = String(rtt);

        berhasil++;

      } else {

        gagal++;
      }

    } else {

      gagal++;
    }

    // ================= STATUS =================
    int status =
      (httpCode >= 200 && httpCode < 300) ? 1 : 0;

    // ================= RSSI =================
    int rssi = WiFi.RSSI();

    // ================= OUTPUT TABEL =================
    Serial.printf(
      "%-8d %-6d %-20s %-8d %-9lu %-10s %-5s %-8d %d\n",
      SCENARIO,
      seq,
      waktuMulaiPesan.c_str(),
      contentLength,
      tKirim,
      terimaStr.c_str(),
      rttStr.c_str(),
      status,
      rssi
    );
  }

  // ================= AKHIR PENGUJIAN =================
  http.end();

  waktuSelesai = millis();

  String waktuAkhirPengujian = getCurrentTime();

  // ================= REKAP =================
  float pdr =
    (float)berhasil / TOTAL_PESAN * 100.0;

  Serial.println();
  Serial.println("========================================");
  Serial.println("           HASIL PENGUJIAN");
  Serial.println("========================================");

  Serial.print("Skenario             : ");
  Serial.println(SCENARIO);

  Serial.print("Waktu mulai          : ");
  Serial.println(waktuMulaiPengujian);

  Serial.print("Waktu selesai        : ");
  Serial.println(waktuAkhirPengujian);

  Serial.print("Total pengujian      : ");
  Serial.println(TOTAL_PESAN);

  Serial.print("Payload              : ");
  Serial.println(PAYLOAD_SIZE);

  Serial.print("Content-Length       : ");
  Serial.println(PAYLOAD_SIZE);

  Serial.print("Berhasil             : ");
  Serial.println(berhasil);

  Serial.print("Gagal                : ");
  Serial.println(gagal);

  Serial.print("PDR                  : ");
  Serial.print(pdr, 2);
  Serial.println("%");

  Serial.print("Durasi pengujian     : ");
  Serial.print(waktuSelesai - waktuMulai);
  Serial.println(" ms");

  Serial.println("========================================");
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);

  delay(1000);

  connectWiFi();

  setupTime();

  runScenario();
}

// ================= LOOP =================
void loop() {
  // Pengujian hanya dijalankan satu kali.
}