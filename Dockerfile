FROM nginx:1.27.0-alpine3.19-perl

EXPOSE 80/tcp

WORKDIR /greenhouse

COPY . /greenhouse

RUN apk update --no-cache && apk add --no-cache libc6-compat \
  && curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/bin sh \
  && arduino-cli config set network.proxy $http_proxy \
  || arduino-cli core install arduino:avr \
  && arduino-cli lib install Ethernet@2.0.2 SD@1.2.4 "DHT sensor library"@1.4.6 "Adafruit TSL2561"@1.1.2 \
  && arduino-cli --no-color compile --verbose --warnings all -b arduino:avr:uno --output-dir ./../generated/build

RUN apk update --no-cache && apk add --no-cache doxygen && doxygen \
  && rm -rf /usr/share/nginx/html/ && cp -r ./generated/html/ /usr/share/nginx/

