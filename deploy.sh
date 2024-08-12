#!/bin/sh

set -ax

cd /var/www/greenhouse &&
	git checkout master &&
	git pull origin master &&
	docker compose up --build -d
