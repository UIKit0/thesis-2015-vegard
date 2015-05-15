#!/bin/sh
qmake fisheye.pro && make clean && make && ./fisheye.app/Contents/MacOS/fisheye
