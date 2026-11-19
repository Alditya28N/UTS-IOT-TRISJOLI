# UTS-IOT-TRISJOLI
## Dokumentasi Proyek Internet of Things (IoT)

Repositori ini berisi program, dataset, dan dokumentasi pengujian proyek Internet of Things (IoT) menggunakan ESP32. Proyek terdiri dari dua bagian utama, yaitu sistem monitoring suhu dan kelembapan kamar serta eksperimen perbandingan kinerja komunikasi HTTP dan MQTT.

Pengujian bertujuan untuk mengetahui karakteristik komunikasi HTTP dan MQTT berdasarkan nilai Round Trip Time (RTT), Packet Delivery Ratio (PDR), dan overhead protokol dengan variasi ukuran payload serta mekanisme Quality of Service (QoS).

---

## 1. Identitas dan Kasus

- **Nama Kelompok:** TriSjoli
- **Kelas:** Gab 2
- **Mata Kuliah:** Internet of Things: Protokol, Platform, dan AI (AAK4WBB3)
- **Program Studi:** S1 Teknik Telekomunikasi
- **Dosen Pengampu:** Zein Hanni Pradana

### Anggota Kelompok

| Nama | NIM | Tanggung Jawab |
|---|---|---|
| Hafid Yustisia Azis | 2311101052 | Perangkat dan Dataset |
| Alditya Nurdiansyah | 2311101056 | Eksperimen Protokol |
| Ferdy Gunanta Ginting | 2311101061 | Analisis dan Laporan |

## 2. Sistem Bagian A — Monitoring Suhu dan Kelembapan

### 2.1 Deskripsi Sistem

Sistem monitoring dirancang menggunakan mikrokontroler ESP32 dan sensor DHT22 untuk mengukur suhu serta kelembapan udara di dalam kamar. Hasil pembacaan sensor diproses oleh ESP32, ditampilkan melalui LCD I2C, kemudian dikirimkan ke platform ThingSpeak menggunakan jaringan Wi-Fi dan protokol HTTP.

Pengamatan dilakukan pada tiga kondisi lingkungan:
1. Normal: kipas mati dan jendela tertutup.
2. Kipas Dinyalakan: kipas menyala dan jendela tertutup.
3. Jendela Dibuka: kipas mati dan jendela terbuka.

### 2.2 Perangkat dan Konfigurasi

| Komponen | Spesifikasi |
|---|---|
| Mikrokontroler | ESP32 |
| Sensor | DHT22 |
| LCD | I2C 16×2 |
| Alamat LCD | 0x27 |
| Pin DHT22 | GPIO 4 |
| Pin SDA | GPIO 21 |
| Pin SCL | GPIO 22 |
| Komunikasi | Wi-Fi IEEE 802.11 |
| Protokol pengiriman | HTTP |
| Platform monitoring | ThingSpeak |
| Interval pengiriman | 30 detik |

### 2.3 Konfigurasi ThingSpeak

Data hasil pembacaan sensor dikirimkan menuju channel ThingSpeak dengan konfigurasi:

- Channel ID: `3576873`
- Field 1: Suhu (°C)
- Field 2: Kelembapan (%RH)
- Fungsi pengiriman: `ThingSpeak.writeFields()`
- Interval pengiriman: 30 detik.

**Link monitoring ThingSpeak:**

https://thingspeak.mathworks.com/channels/3576873

### 2.4 Periode dan Hasil Perekaman Dataset

Perekaman data dilakukan pada 12–14 November 2026 dengan interval pengiriman nominal 30 detik.

| Parameter | Hasil |
|---|---:|
| Waktu mulai | 12 November 2026, 21:07:01 WIB |
| Waktu selesai | 14 November 2026, 20:58:55 WIB |
| Interval pengiriman | 30 detik |
| Jumlah data yang diharapkan | 5.744 |
| Jumlah data diterima | 2.188 |
| Kelengkapan dataset | 38,09% |

### 2.5 Pelabelan Kondisi Lingkungan

Pelabelan dilakukan berdasarkan pengamatan langsung terhadap status kipas dan posisi jendela.

| Kelas | Kondisi | Jumlah Kejadian |
|---|---|---:|
| Normal | Kipas mati, jendela tertutup | 6 |
| Kipas Dinyalakan | Kipas menyala, jendela tertutup | 6 |
| Jendela Dibuka | Kipas mati, jendela terbuka | 6 |
| **Total** | | **18** |

File pelabelan kejadian: `label_log.xlsx` dan `label_log.csv`.

### 2.6 Validasi dan Keterbatasan

Validasi sensor dilakukan dengan membandingkan pembacaan DHT22 terhadap termometer suhu ruang yang dilengkapi pengukur kelembapan.

Hasil pemeriksaan:
- Suhu DHT22: 25,8 °C.
- Suhu alat pembanding: 25,9 °C.
- Kelembapan DHT22: 61,7%.
- Kelembapan alat pembanding: 60%.

Terdapat gangguan pemadaman listrik pada 14 November 2026 pukul 17.35.20–18.22.20 WIB saat pengamatan Kelas 2 kejadian ke-6. Gangguan menyebabkan pengamatan hanya berlangsung selama 47 menit.

---

## 3. Eksperimen Bagian B — Pengujian HTTP dan MQTT

### 3.1 Tujuan Pengujian

Eksperimen dilakukan untuk membandingkan kinerja protokol HTTP dan MQTT pada ESP32 berdasarkan:
- Round Trip Time (RTT).
- Packet Delivery Ratio (PDR).
- Overhead protokol.
- Pengaruh ukuran payload.
- Pengaruh penggunaan QoS pada MQTT.

### 3.2 Perangkat dan Konfigurasi Pengujian

Pengujian menggunakan ESP32 DevKit yang disimulasikan melalui Wokwi.

| Parameter | Konfigurasi |
|---|---|
| Platform simulasi | Wokwi |
| Jaringan Wi-Fi | Wokwi-GUEST |
| Password Wi-Fi | Kosong |
| HTTP Server | http://httpbin.org/post |
| HTTP Port | 80 |
| MQTT Broker | broker.hivemq.com |
| MQTT Port | 1883 |
| MQTT Topic | trisjoli/uts/iot/echo |
| Jumlah pesan | 100 pesan per skenario |
| Interval pengiriman | 2 detik |
| Timeout | 5.000 ms |
| Ukuran payload | 20 byte dan 500 byte |
| Keamanan komunikasi | Tanpa TLS |

### 3.3 Enam Skenario Pengujian

| Skenario | Protokol | QoS | Payload |
|---|---|---|---|
| S1 | HTTP POST | - | 20 byte |
| S2 | HTTP POST | - | 500 byte |
| S3 | MQTT | 0 | 20 byte |
| S4 | MQTT | 0 | 500 byte |
| S5 | MQTT | 1 | 20 byte |
| S6 | MQTT | 1 | 500 byte |

### 3.4 Metode Pengukuran

Pengukuran RTT menggunakan fungsi `millis()` pada ESP32 dengan mencatat waktu pengiriman dan penerimaan pesan.

- **HTTP:** RTT dihitung sejak permintaan HTTP POST dikirim hingga seluruh respons server diterima dan dibaca oleh ESP32. Pengujian menggunakan HTTP Keep-Alive.
- **MQTT:** RTT dihitung menggunakan mekanisme publish-echo, yaitu sejak pesan dipublikasikan hingga pesan diterima kembali melalui topic yang sama.
- **MQTT QoS 1:** RTT dihitung berdasarkan penerimaan pesan echo, bukan penerimaan paket PUBACK.

Setiap pesan memiliki nomor urut dari 1 hingga 100. Pesan yang tidak menerima respons dalam waktu 5.000 ms dinyatakan gagal atau hilang.

Sinkronisasi waktu menggunakan Network Time Protocol (NTP) dengan zona waktu WIB (UTC+07:00). Nilai RSSI dicatat menggunakan fungsi `WiFi.RSSI()`.

### 3.5 Jadwal Pengujian

Seluruh skenario dilaksanakan pada 15 November 2026.

| Skenario | Waktu Pengujian (WIB) |
|---|---|
| S1 | 18.37.11–18.40.29 |
| S2 | 19.05.32–19.08.51 |
| S3 | 19.56.20–20.00.16 |
| S4 | 20.10.11–20.13.49 |
| S5 | 20.22.14–20.25.51 |
| S6 | 20.32.03–20.35.58 |

---

## 4. Metode Perhitungan dan Analisis

Pengolahan data eksperimen dilakukan menggunakan Microsoft Excel berdasarkan log hasil pengujian dan pengamatan paket menggunakan Wireshark.

### 4.1 Packet Delivery Ratio (PDR)

PDR merupakan persentase pesan yang berhasil diterima dibandingkan dengan seluruh pesan yang dikirimkan.

Rumus:

`PDR = (Jumlah Pesan Diterima / Jumlah Pesan Dikirim) × 100%`

Perhitungan menggunakan Microsoft Excel.

### 4.2 Round Trip Time (RTT)

RTT merupakan waktu yang dibutuhkan sejak pesan dikirim hingga respons diterima kembali.

Parameter statistik yang dihitung:
- Mean RTT menggunakan `AVERAGE()`.
- Median RTT menggunakan `MEDIAN()`.
- Simpangan baku menggunakan `STDEV.S()`.
- Persentil ke-95 menggunakan `PERCENTILE.INC(range,0.95)`.

Hanya pesan yang berhasil menerima respons dalam batas timeout 5.000 ms yang dimasukkan ke dalam perhitungan statistik RTT.

### 4.3 Overhead Protokol

Overhead dihitung berdasarkan ukuran tambahan header protokol terhadap ukuran payload aplikasi.

Rumus:

`Overhead (%) = (Total Ukuran Header / Ukuran Payload) × 100%`

Pengukuran dilakukan pada lapisan aplikasi dengan arah trafik keluar dari ESP32.

**Komponen overhead HTTP:**
- Request Line.
- Host.
- User-Agent.
- Connection.
- Accept-Encoding.
- Content-Type.
- Content-Length.
- Header Separator.

**Komponen overhead MQTT:**
- Control Packet Type.
- Remaining Length.
- Topic Length.
- Topic Name.
- Packet Identifier untuk QoS 1.

Ukuran komponen header diperoleh berdasarkan pengamatan paket menggunakan Wireshark. Perhitungan overhead aplikasi tidak mencakup header TCP/IP maupun IEEE 802.11.

---

## 5. Hasil Pengujian dan Analisis

### 5.1 Hasil Enam Skenario

Berikut merupakan hasil pengujian berdasarkan laporan kelompok.

| Skenario | Sent | Recv | PDR (%) | Mean RTT (ms) | Median (ms) | SD (ms) | P95 (ms) | Overhead (%) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| S1 | 100 | 100 | 100 | 586,20 | 559 | 113,39 | 672,5 | 9,6 |
| S2 | 100 | 100 | 100 | 598,71 | 579 | 81,36 | 758 | 0,386 |
| S3 | 100 | 100 | 100 | 373,95 | 379 | 15,41 | 381,45 | 1,25 |
| S4 | 100 | 100 | 100 | 202,73 | 196 | 21,22 | 229 | 0,052 |
| S5 | 100 | 100 | 100 | 197,17 | 195 | 26,97 | 229 | 1,35 |
| S6 | 100 | 100 | 100 | 365,37 | 363 | 7,49 | 380 | 0,056 |

### 5.2 Interpretasi Hasil

Berdasarkan hasil pengujian, seluruh skenario memperoleh PDR sebesar 100%. Hal ini menunjukkan bahwa seluruh pesan berhasil menerima respons dalam batas timeout 5.000 ms.

Pada pengukuran RTT, HTTP menghasilkan rata-rata sebesar 586,20 ms pada payload 20 byte dan 598,71 ms pada payload 500 byte.

Sementara itu, MQTT menghasilkan rata-rata RTT sebesar:
- QoS 0: 373,95 ms dan 202,73 ms.
- QoS 1: 197,17 ms dan 365,37 ms.

Hasil tersebut menunjukkan bahwa MQTT memiliki rata-rata RTT lebih rendah dibandingkan HTTP pada pengujian yang dilakukan.

Penggunaan payload yang lebih besar tidak selalu menyebabkan peningkatan RTT. Hal ini dapat dipengaruhi oleh kondisi jaringan dan waktu respons server maupun broker selama pengujian.

### 5.3 Rekomendasi

Berdasarkan hasil eksperimen, MQTT dapat dipertimbangkan untuk komunikasi data monitoring secara berkala karena menghasilkan RTT lebih rendah dibandingkan HTTP pada skenario pengujian.

MQTT QoS 1 dapat digunakan apabila pengiriman data membutuhkan konfirmasi penerimaan pesan, sedangkan QoS 0 dapat digunakan untuk komunikasi yang tidak memerlukan konfirmasi pada tingkat protokol.

Hasil tersebut tetap memiliki keterbatasan karena pengujian menggunakan Wokwi, HTTPBin, dan HiveMQ Public Broker dengan endpoint yang berbeda.

---

## 6. Struktur Repositori

```text
UTS-IOT-TRISJOLI/
│
├── CSV
├── KODE
├── README.md
```

### Keterangan File

| File | Fungsi |
|---|---|
| PROJECT A.ino | Program monitoring suhu dan kelembapan menggunakan ESP32, DHT22, LCD I2C, dan ThingSpeak. |
| S1.ino | Pengujian HTTP dengan payload 20 byte. |
| S2.ino | Pengujian HTTP dengan payload 500 byte. |
| S3.ino | Pengujian MQTT QoS 0 dengan payload 20 byte. |
| S4.ino | Pengujian MQTT QoS 0 dengan payload 500 byte. |
| S5.ino | Pengujian MQTT QoS 1 dengan payload 20 byte. |
| S6.ino | Pengujian MQTT QoS 1 dengan payload 500 byte. |

---

## 7. Reproduksibilitas dan Verifikasi

### 7.1 Perangkat Lunak dan Tools

Perangkat lunak dan tools yang digunakan dalam pengerjaan proyek meliputi:

- Arduino IDE untuk pemrograman ESP32.
- Wokwi untuk simulasi ESP32 dan pengujian komunikasi.
- ThingSpeak untuk penyimpanan dan pemantauan data sensor.
- Wireshark untuk pengamatan paket dan perhitungan overhead.
- Microsoft Excel untuk pengolahan data dan perhitungan statistik.

### 7.2 Cara Menjalankan Program

**A. Monitoring Suhu dan Kelembapan**

1. Buka Arduino IDE.
2. Pilih board ESP32.
3. Buka file `PROJECT A.ino`.
4. Sesuaikan konfigurasi Wi-Fi dan ThingSpeak.
5. Hubungkan DHT22 dan LCD I2C sesuai konfigurasi pin.
6. Upload program ke ESP32.
7. Buka Serial Monitor dengan baud rate 115200.
8. Amati hasil pembacaan sensor dan pengiriman data ke ThingSpeak.

**B. Pengujian HTTP dan MQTT**

1. Buka Arduino IDE.
2. Pilih board ESP32.
3. Pilih salah satu program S1 sampai S6.
4. Pastikan konfigurasi Wi-Fi, server, atau broker sesuai.
5. Jalankan program melalui ESP32 atau lingkungan simulasi Wokwi.
6. Amati proses pengiriman dan penerimaan pesan.
7. Catat hasil pengujian melalui Serial Monitor.
8. Ulangi langkah yang sama untuk seluruh skenario.

### 7.3 Dataset dan Log Pengujian

Data mentah dan dokumentasi pengujian dapat diakses melalui tautan berikut.

**Dataset monitoring suhu dan kelembapan:**

[File CSV Dataset Monitoring]([https://docs.google.com/spreadsheets/d/1FxFMRrEN_d9MPORNsCe8fwt1RcoD4ss5/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQCMVSG_WVsEQY-wjI3y0M64AS-bPVxOs0ld1rcP85OZNeg?e=oIqoxh))

**Log pelabelan kondisi lingkungan:**

[Label Log]([https://docs.google.com/spreadsheets/d/1lr8nAMArm7b8cxJ5eUcZ67XOFNYvdKow/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQCIIx9dPXSjTZzT8OQWM5qlAStSCNXR_GEZgX4gycA1mJQ?e=DERgTT))

**Log eksperimen HTTP dan MQTT:**

| Skenario | File Pengujian |
|---|---|
| S1 | [exp_S1.csv]([https://docs.google.com/spreadsheets/d/1_9J7FCpdCzWg-rIVsn4faJR4vk19Aw4c/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQAxJUv4I-3aTabF9ETW8wWRAbo7GppweShjDx9oJTQpWxw?e=TzvK2D)) |
| S2 | [exp_S2.csv]([https://docs.google.com/spreadsheets/d/1cymeHkC0Xq0wjGLJvu_lVrB98c_GU-iZ/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQAZw7fSWyALTawcb0SqCB1bAbwpZlMJMH7kNyDoSju16-I?e=MVOWGV)) |
| S3 | [exp_S3.csv]([https://docs.google.com/spreadsheets/d/16XN2ROnkpukCEVk3w3_bQLK9_iXNybyI/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQCu-xh4aikgSaZyoBkqpP_uAT5-XdOJWSgV7cf_hqTLfOs?e=cD6aMM)) |
| S4 | [exp_S4.csv]([https://docs.google.com/spreadsheets/d/1kFf2wTi_I2pd4udQTX5V-LMVAWBuTzSL/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQDpPWPFQ0GzQbTvIEqN3PvjAfopUpgowEFSYnqzr-B_kFs?e=xQJ9Hp)) |
| S5 | [exp_S5.csv]([https://docs.google.com/spreadsheets/d/1rws-zxY1LLsB7afSlUGHScEIV_M2WYvW/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQAEB1AzWAJ4RJ8K0kTfdqiZAfH9RL-bsYrIUe6_f68VgEM?e=NJGL8R)) |
| S6 | [exp_S6.csv]([https://docs.google.com/spreadsheets/d/19EaS6X6JiruDWoZkFCAVT1PMCc1lMNKI/edit?usp=drive_link](https://1drv.ms/x/c/4cefb4e17277a950/IQD2GLzIE1ANSKn968i87hRvAZdLloxHq4IgiBy39FLGHPE?e=4SPZSe)) |

**Folder pengumpulan data:**

https://drive.google.com/drive/folders/1Fh-ztkqiblh9udcSIUInvx7h32VvQ3S6?usp=drive_link

### 7.4 Verifikasi Hasil

Verifikasi dilakukan dengan menjalankan kembali program ESP32, mengamati komunikasi melalui Serial Monitor, serta mencocokkan hasil pengujian dengan data mentah CSV.

Pengamatan paket menggunakan Wireshark dilakukan untuk memperoleh ukuran komponen header HTTP dan MQTT yang digunakan dalam perhitungan overhead.

---

## 8. Dokumentasi dan Kontribusi

### 8.1 Log Penggunaan AI

ChatGPT digunakan sebagai alat bantu dalam proses pengerjaan proyek, meliputi:

1. Membantu pembuatan program ESP32 untuk pembacaan sensor, LCD, dan pengiriman data ke ThingSpeak.
2. Membantu pembuatan program pengujian HTTP dan MQTT dengan variasi payload serta QoS.
3. Membantu memahami konfigurasi dan penggunaan platform ThingSpeak.
4. Membantu penerapan rumus perhitungan RTT dan PDR menggunakan Microsoft Excel.
5. Membantu memahami metode perhitungan overhead HTTP dan MQTT.
6. Membantu mengidentifikasi informasi paket melalui Wireshark.

Seluruh hasil penggunaan AI diperiksa dan diverifikasi melalui pengujian program, pencocokan data, serta pemeriksaan paket komunikasi.

### 8.2 Kontribusi Anggota

| Nama | Kontribusi | Tugas |
|---|---:|---|
| Hafid Yustisia Azis | 100% | Perangkat dan Dataset |
| Alditya Nurdiansyah | 100% | Eksperimen Protokol |
| Ferdy Gunanta Ginting | 100% | Analisis dan Laporan |

---

## 9. Dokumentasi dan Luaran Proyek

| No. | Luaran | Status / Tautan |
|---|---|---|
| 1 | Laporan UTS IoT | Tersedia |
| 2 | Repositori GitHub | Tersedia |
| 3 | Dataset monitoring dan log eksperimen | Tersedia |
| 4 | Channel ThingSpeak | Tersedia |
| 5 | Video demonstrasi | Tersedia |
| 6 | Log penggunaan AI | Tersedia dalam laporan |
| 7 | Form kontribusi anggota | Tersedia dalam laporan |

**Video demonstrasi:**

https://drive.google.com/file/d/1E1e_ZenLYo-afCCz0kww_L4xQxLFOm_s/view?usp=drive_link

---

## 10. Keterbatasan Pengujian

Beberapa keterbatasan yang ditemukan selama pelaksanaan proyek adalah:

1. Perekaman dataset monitoring hanya berlangsung sekitar dua hari dan belum mencapai target tujuh hari.
2. Kelengkapan dataset mencapai 38,09% dari jumlah data yang diharapkan.
3. Terjadi pemadaman listrik yang menyebabkan salah satu kejadian pengamatan hanya berlangsung selama 47 menit.
4. Pengujian HTTP dan MQTT menggunakan endpoint serta server publik yang berbeda.
5. Pengujian protokol dilakukan melalui lingkungan simulasi Wokwi sehingga kondisi RSSI tidak sepenuhnya merepresentasikan jaringan Wi-Fi pada perangkat fisik.
6. Perbedaan kondisi jaringan, waktu pengujian, dan respons server atau broker dapat memengaruhi hasil RTT.

---

## 11. Link Repositori

**GitHub Repository:**

https://github.com/Alditya28N/UTS-IOT-TRISJOLI

Repositori ini memuat program monitoring suhu dan kelembapan, program eksperimen HTTP dan MQTT, serta dokumentasi pelaksanaan proyek UTS Internet of Things (IoT) Kelompok TriSjoli.
