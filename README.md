# UTS-IOT-TRISJOLI
# UTS-IOT-TRISJOLI
## Dokumentasi Proyek Internet of Things (IoT)

### 1. Deskripsi Proyek

Repositori ini berisi program dan dokumentasi pengujian sistem Internet of Things (IoT) menggunakan ESP32. Proyek ini terdiri dari dua bagian utama, yaitu sistem monitoring suhu dan kelembapan serta pengujian kinerja komunikasi menggunakan protokol HTTP dan MQTT.

Tujuan pengujian adalah mengetahui dan membandingkan karakteristik komunikasi HTTP dan MQTT berdasarkan ukuran payload dan Quality of Service (QoS).

### 2. Struktur Repositori

File yang tersedia dalam repositori ini adalah:

```text
UTS-IOT-TRISJOLI/
│
├── README.md
├── PROJECT A.ino
├── S1.ino
├── S2.ino
├── S3.ino
├── S4.ino
├── S5.ino
└── S6.ino
```

Keterangan:
- `PROJECT A.ino`: Program monitoring suhu dan kelembapan menggunakan ESP32, sensor DHT22, LCD I2C, dan ThingSpeak.
- `S1.ino`: Pengujian HTTP dengan payload 20 byte.
- `S2.ino`: Pengujian HTTP dengan payload 500 byte.
- `S3.ino`: Pengujian MQTT dengan QoS 0 dan payload 20 byte.
- `S4.ino`: Pengujian MQTT dengan QoS 0 dan payload 500 byte.
- `S5.ino`: Pengujian MQTT dengan QoS 1 dan payload 20 byte.
- `S6.ino`: Pengujian MQTT dengan QoS 1 dan payload 500 byte.

### 3. Perangkat Keras dan Perangkat Lunak

**Perangkat keras:**
- ESP32 sebagai mikrokontroler.
- Sensor DHT22 untuk membaca suhu dan kelembapan.
- LCD I2C 16x2 untuk menampilkan hasil pembacaan sensor.

**Perangkat lunak dan layanan:**
- Arduino IDE untuk pemrograman ESP32.
- Wokwi sebagai lingkungan simulasi yang digunakan pada konfigurasi Wi-Fi pengujian.
- ThingSpeak untuk pengiriman dan pemantauan data sensor.
- HTTPBin sebagai endpoint pengujian HTTP.
- HiveMQ Public Broker sebagai server komunikasi MQTT.

### 4. Dependensi dan Library

Library yang digunakan berdasarkan program:

| Library | Kegunaan |
|---|---|
| WiFi.h | Menghubungkan ESP32 ke jaringan Wi-Fi |
| HTTPClient.h | Komunikasi menggunakan protokol HTTP |
| AsyncMqttClient.h | Komunikasi menggunakan protokol MQTT |
| ThingSpeak.h | Mengirim data ke ThingSpeak |
| DHT.h | Membaca sensor DHT22 |
| Wire.h | Komunikasi I2C |
| LiquidCrystal_I2C.h | Mengendalikan LCD I2C |
| time.h | Sinkronisasi dan pengaturan waktu |
| string.h | Pengolahan data berbasis karakter |

Catatan: WiFi, HTTPClient, Wire, dan time merupakan bagian dari lingkungan pengembangan ESP32. Library lainnya perlu dipasang sesuai kebutuhan program. Versi Arduino IDE, ESP32 Board Package, dan library yang benar-benar digunakan perlu dicatat berdasarkan lingkungan pengujian.

### 5. Konfigurasi Sistem

**A. Konfigurasi Monitoring Suhu dan Kelembapan**

Pengaturan pin pada program PROJECT A:

| Komponen | Pin ESP32 |
|---|---|
| DHT22 Data | GPIO 4 |
| LCD SDA | GPIO 21 |
| LCD SCL | GPIO 22 |
| Alamat LCD I2C | 0x27 |

Interval pengiriman data ke ThingSpeak adalah 30 detik.

Sebelum menjalankan program, sesuaikan konfigurasi berikut:
- Nama Wi-Fi (SSID).
- Password Wi-Fi.
- Channel ID ThingSpeak.
- Write API Key ThingSpeak.

**B. Konfigurasi Pengujian HTTP**

Parameter yang digunakan:
- Server: http://httpbin.org/post
- Jumlah pesan: 100 pesan.
- Interval pengiriman: 2.000 ms.
- Timeout: 5.000 ms.
- Ukuran payload: 20 byte dan 500 byte.

**C. Konfigurasi Pengujian MQTT**

Parameter yang digunakan:
- Broker: broker.hivemq.com
- Port: 1883.
- Topic: trisjoli/uts/iot/echo
- Jumlah pesan: 100 pesan.
- Interval pengiriman: 2.000 ms.
- Timeout: 5.000 ms.

### 6. Skenario Pengujian

Pengujian dilakukan menggunakan enam skenario berikut:

| Skenario | Protokol | QoS | Payload |
|---|---|---|---|
| S1 | HTTP | - | 20 byte |
| S2 | HTTP | - | 500 byte |
| S3 | MQTT | 0 | 20 byte |
| S4 | MQTT | 0 | 500 byte |
| S5 | MQTT | 1 | 20 byte |
| S6 | MQTT | 1 | 500 byte |

Setiap skenario dijalankan secara terpisah menggunakan program ESP32 yang sesuai.

### 7. Langkah Menjalankan Program

**A. Menjalankan Program Monitoring IoT**

1. Buka Arduino IDE.
2. Pilih board ESP32 yang sesuai.
3. Pasang library yang diperlukan.
4. Buka file PROJECT A.ino.
5. Sesuaikan konfigurasi Wi-Fi dan ThingSpeak.
6. Hubungkan sensor DHT22 dan LCD I2C sesuai konfigurasi pin.
7. Hubungkan ESP32 ke komputer.
8. Upload program ke ESP32.
9. Buka Serial Monitor dengan baud rate 115200.
10. Amati hasil pembacaan suhu dan kelembapan pada LCD dan Serial Monitor.
11. Periksa data yang berhasil dikirim ke ThingSpeak.

**B. Menjalankan Pengujian HTTP dan MQTT**

1. Buka Arduino IDE.
2. Pilih board ESP32.
3. Pasang library AsyncMqttClient untuk pengujian MQTT.
4. Pilih salah satu file skenario S1 sampai S6.
5. Pastikan konfigurasi Wi-Fi dan server sesuai.
6. Upload program ke ESP32.
7. Buka Serial Monitor.
8. Amati proses pengiriman dan penerimaan pesan.
9. Catat hasil pengujian setiap skenario.
10. Ulangi langkah yang sama untuk skenario berikutnya.

### 8. Parameter Pengamatan

Parameter yang diamati dalam pengujian komunikasi HTTP dan MQTT meliputi:

- Jumlah pesan yang berhasil dikirim.
- Jumlah pesan yang gagal.
- Waktu pengiriman dan penerimaan pesan.
- Ukuran payload.
- Ukuran data komunikasi atau overhead apabila diukur.
- Pengaruh penggunaan QoS terhadap hasil komunikasi.

Hasil pengujian digunakan sebagai dasar perbandingan performa protokol HTTP dan MQTT.

### 9. Reproduksibilitas dan Verifikasi

Untuk memastikan pengujian dapat dilakukan kembali, setiap skenario menggunakan konfigurasi parameter yang telah ditentukan dalam program.

Verifikasi dilakukan dengan menjalankan kembali program pada ESP32 dan mengamati hasil komunikasi melalui Serial Monitor.

Data hasil pengujian dapat didokumentasikan dalam bentuk tabel atau file CSV. Grafik hasil pengujian dapat dibuat berdasarkan data yang telah dikumpulkan.

Versi program dan commit GitHub yang digunakan sebagai acuan pengujian perlu dicatat agar perubahan kode dapat ditelusuri.

### 10. Keamanan Konfigurasi

Informasi yang bersifat rahasia, seperti password Wi-Fi dan API Key ThingSpeak, tidak boleh dibagikan dalam repositori publik.

Gunakan konfigurasi masing-masing pengguna ketika menjalankan program. Hindari mengunggah kredensial pribadi ke GitHub.

### 11. Link Repositori

GitHub Repository:

https://github.com/Alditya28N/UTS-IOT-TRISJOLI

Seluruh kode program yang digunakan dalam proyek ini dapat diakses melalui repositori tersebut.
