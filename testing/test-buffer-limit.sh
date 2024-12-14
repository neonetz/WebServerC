#!/bin/bash

# File: test_request.sh
# Deskripsi: Script untuk menguji batas ukuran permintaan dengan curl

MAX_REQUEST_SIZE=5000  # Ukuran batas yang telah ditentukan
URL="http://localhost:8080/"  # Ubah dengan URL server yang sesuai

# Membuat payload besar melebihi MAX_REQUEST_SIZE
PAYLOAD=$(printf 'A%.0s' $(seq 1 $MAX_REQUEST_SIZE))

# Mengirimkan permintaan dengan curl dan memeriksa status respon
response=$(curl -s -o /dev/null -w "%{http_code}" -X POST --data "$PAYLOAD" $URL)

if [ "$response" -eq 413 ]; then
    echo "Test passed: Server menolak permintaan dengan status 413 (Payload Too Large)."
else
    echo "Test failed: Server tidak menanggapi dengan status 413 seperti yang diharapkan. Respon: $response"
fi