#!/bin/bash

while [ true ]; do
  curl --verbose -H 'Accept:' -H 'User-Agent:' 10.0.0.2
  sleep 0.1
  clear
done
