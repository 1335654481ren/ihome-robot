#!/bin/sh
#cat /proc/bus/input/devices
gcc uinput-keyboard.c -o uinput-keyboard
gcc uinput-mouse.c -o uinput-mouse
gcc control.c -o control
gcc touch-simulate.c -o touch-simulate
gcc move-mouse.c -o move-mouse