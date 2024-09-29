#!/bin/bash

while [ true ]; do
  curl -H 'Accept:' -H 'User-Agent:' 192.168.0.64 | tee -a data.json && echo >>data.json

  # curl -H 'Accept:' -H 'User-Agent:' 192.168.0.64
  sleep 0.1
  clear
done
