#!/bin/bash

URL="http://localhost:8080/newroute"

response=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE $URL)

if [ "$response" -eq 200 ]; then
    echo "Test passed: Server responded with status 200 (OK)."
else
    echo "Test failed: Server responded with status $response."
fi