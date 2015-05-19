# Project for an Arduino-controlled 1-lane Pinewood Derby practice track

This is just the timing and starting gate controls, you have to supply a track. I used 
BSOA practice track and a PVC frame (http://www.abc-pinewood-derby.com/track-stand.php).
I cut out a slot in the middle of the topmost track section for a starting gate, then a 7mm
square hole in the middle of the track for a finish line, through which an IR light beam
shines and signals the Arduino when a car crosses the finish (I used this IR pair 
https://www.sparkfun.com/products/241and this basic circuit: 
http://www.reconnsworld.com/ir_ultrasonic_basicirdetectemit.html)

The Arduino controls an LCD display with buttons (https://www.adafruit.com/products/714).
Pressing the SELECT button starts a race by lowering the starting gate with a servo, waiting
for the IR beam to be broken at the finish, and displaying the time between the two. The race
can be reset with the LEFT (Back) button.

## This project requires the following arduino libraries:
* https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library
* http://www.arduino.cc/en/Reference/Servo
* http://www.arduino.cc/en/reference/Wire

## 3D Files
STL and Inventor source files are included for the following physical parts:
* <b>Servo Gate</b>: Attaches to a standard size servo and controls car starting
* <b>Servo Bracket</b>: Servo mount for attaching beneath the track
* <b>Finish Line</b>: Mount for IR emitter and detector. Requires a 7mm square hole
  in the track at the finish line where the bottom is mounted under.