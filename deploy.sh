#!/bin/sh

set -ax

whoami

cd /var/www/greenhouse &&
  git restore . &&
  git checkout master &&
  git reset --hard origin/master &&
  git pull origin master &&
  docker compose up --build -d
