#!/bin/sh
sudo cp * /usr/local/lib/
sudo apt-get install libsndfile-dev

sudo ldconfig -v
