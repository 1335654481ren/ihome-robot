#!/bin/sh
#sudo apt-get install festival-dev libsndfile-devel
#./configure CXXFLAGS="-DNO_SSE" --without-pulseaudio
#./configure --prefix=$PWD/install CXXFLAGS="-DNO_SSE"
./configure --prefix=/usr/local --enable-festival CXXFLAGS="-DNO_SSE"
