"""
This file is part of "arduino_family_calendar".

"arduino_family_calendar" is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"arduino_family_calendar" is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "arduino_family_calendar".  If not, see <https://www.gnu.org/licenses/>.
"""

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
