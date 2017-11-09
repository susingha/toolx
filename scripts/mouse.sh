#!/bin/bash


# Fedora
xinput set-int-prop "DualPoint Stick" "Evdev Wheel Emulation" 8 1
xinput set-int-prop "DualPoint Stick" "Evdev Wheel Emulation Button" 8 2
xinput set-int-prop "DualPoint Stick" "Evdev Wheel Emulation Axes" 8 6 7 4 5
xinput set-int-prop "DualPoint Stick" "Evdev Wheel Emulation Timeout" 16 0

# Ubuntu
xinput set-int-prop "AlpsPS/2 ALPS DualPoint TouchPad" "Evdev Wheel Emulation" 8 1
xinput set-int-prop "AlpsPS/2 ALPS DualPoint TouchPad" "Evdev Wheel Emulation Button" 8 2
xinput set-int-prop "AlpsPS/2 ALPS DualPoint TouchPad" "Evdev Wheel Emulation Axes" 8 6 7 4 5
xinput set-int-prop "AlpsPS/2 ALPS DualPoint TouchPad" "Evdev Wheel Emulation Timeout" 16 0
