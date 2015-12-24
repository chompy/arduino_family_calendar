import serial
import time
import sys
import array

ser = serial.Serial('COM4', 9600, timeout=0)

time.sleep(3)

print( ser.write( array.array("B", [1]).tostring() ) )

while 1:
    byte = ser.read(1)
    if byte:
        print "RECV: " + line
    if byte and ord(byte) == 1:
        print "SEND TIME"
    
        
        sys.exit()
