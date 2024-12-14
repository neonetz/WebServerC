# WebServerC

## Deskripsi Proyek

WebServerC adalah proyek yang bertujuan untuk membuat server web sederhana menggunakan bahasa pemrograman C. Proyek ini menunjukkan cara mengatur server web dasar yang dapat menangani permintaan HTTP dan memberikan respons yang sesuai. Server ini dirancang untuk mendemonstrasikan konsep dasar pemrograman jaringan, pengelolaan proses, dan penanganan sinyal di C.

## Fitur Utama

- Menangani permintaan HTTP GET dan POST.
- Menolak permintaan yang melebihi batas ukuran yang ditentukan dengan kode status HTTP 413 (Payload Too Large).
- Mengelola rute untuk mengarahkan permintaan ke file statis atau halaman HTML.
- Menggunakan proses anak untuk menangani beberapa klien secara bersamaan.
- Menyediakan logging untuk permintaan dan fork proses.

## Struktur Proyek

```plaintext
WebServerC/
├── README.md
├── src/
│   ├── ClientHandler.c
│   ├── HTTP_Server.c
│   ├── Logger.c
│   ├── main.c
│   ├── Response.c
│   |── Routes.c
|   |── Server.c
|   └── SignalHandler.c
|
├── testing/
│   ├── test-buffer-limit.sh
│   └── test-buffer-overflow.sh
└── include/
    ├── ClientHandler.h
    ├── HTTP_Server.h
    ├── Logger.h
    ├── Response.h
    ├── Routes.h
    ├── Server.h
    └── SignalHanler.h
```

## Instalasi

### Clone Repository

```bash
git clone https://github.com/username/WebServerC.git
cd WebServerC
```

### Kompilasi Kode

```bash
make
```

### Jalankan Server

```bash
./server.o
```

## Penggunaan

Setelah server berjalan, Anda dapat mengirimkan permintaan HTTP menggunakan alat seperti `curl` atau browser web. Contoh permintaan menggunakan `curl`:

```bash
curl http://localhost:8080
```

### Menguji Batas Ukuran Permintaan

Untuk menguji batas ukuran permintaan, Anda dapat menjalankan skrip berikut:

```bash
bash testing/test-buffer-limit.sh
```

### Menguji Buffer Overflow

Untuk menguji potensi buffer overflow, jalankan:

```bash
bash testing/test-buffer-overflow.sh
```

## Penjelasan Fungsi Utama

### `start_server(int port)`

Fungsi ini menginisialisasi server dan mulai mendengarkan permintaan pada port yang ditentukan. Ini juga mengatur rute dan menangani klien yang terhubung.

### `handle_client(int client_socket, struct Route *route)`

Fungsi ini bertanggung jawab untuk menangani komunikasi dengan klien. Ini membaca pesan dari klien, memeriksa apakah permintaan valid, dan mengirimkan respons yang sesuai. Jika ukuran permintaan melebihi batas, server akan mengirimkan kode status 413.

### `init_server(HTTP_Server * http_server, int port)`

Fungsi ini menginisialisasi socket server dan mengikatnya ke alamat dan port yang ditentukan. Ini juga mempersiapkan server untuk mendengarkan permintaan masuk.

### `sigchld_handler(int signo)`

Fungsi ini menangani sinyal SIGCHLD untuk membersihkan proses anak yang telah selesai, mencegah terjadinya zombie process.

## Lisensi

## Author

- Muhammad Adhyaksa Fadillah (NIM: 231524051)
- Ais Laksana (NIM: 231524035)

## Kontak

Jika Anda memiliki pertanyaan atau saran, silakan hubungi kami melalui email atau melalui platform pengembangan yang Anda gunakan.

Dengan dokumentasi yang lengkap dan jelas ini, diharapkan pengguna dapat dengan mudah memahami dan menggunakan proyek WebServerC. Selamat mencoba!
