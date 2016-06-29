#!/bin/bash

xdotool search "outputs.txt" windowactivate
for i in {1..160}
  do
  		xdotool key ctrl+Left
 done
for i in {1..154}
  do
     xdotool key ctrl+Right
     xdotool key Delete
     xdotool key Return
 done