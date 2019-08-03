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

import sys
import os
import array
from PIL import Image, ImageFont, ImageDraw

FONT_W = 6
FONT_H = 8

# define constant vars
FONT_PATH = os.path.join( os.path.dirname(os.path.realpath(__file__)), "fonts" )

# make sure font dir exists
if not os.path.exists(FONT_PATH) or not os.path.isdir(FONT_PATH):
    print "Font directory not found."
    sys.exit()

# create base image
im = Image.new("RGB", (256, 256), (255, 255, 255))
draw = ImageDraw.Draw(im)

# iterate all fonts
for filename in os.listdir(FONT_PATH):

    fullpath = os.path.join( FONT_PATH, filename )

    font = ImageFont.truetype(fullpath, 5)

    y = 0
    for i in range(0, 255):
        draw.text(( (i % 16) * FONT_W , y * FONT_H ))
        if i > 0 and i % 16 == 0:
            y += 1

    im.save( os.path.splitext(filename)[0] + ".bmp" )
