import sys
import os
import array
from PIL import Image

MAX_ICON_W = 64
MAX_ICON_H = 64
BG_COLOR = (0,0,0)

# define constant vars
ICON_PATH = os.path.join( os.path.dirname(os.path.realpath(__file__)), "icons" )

# make sure icon dir exists
if not os.path.exists(ICON_PATH) or not os.path.isdir(ICON_PATH):
    print "Icon directory not found."
    sys.exit()

# collect data
data = ""

# get number of icons
data += array.array("B", [len( os.listdir(ICON_PATH) )]).tostring()

# iterate all photos
for filename in os.listdir(ICON_PATH):
    
    fullpath = os.path.join( ICON_PATH, filename )

    # open photo
    im = Image.open(fullpath)

    # resize
    if im.size[0] > MAX_ICON_W or im.size[1] > MAX_ICON_H:
        im.thumbnail((MAX_ICON_W, MAX_ICON_H))

    # replace bg
    bg = Image.new("RGB", im.size, BG_COLOR)
    if im.mode == "RGBA":
        bg.paste(im, im)
    else:
        bg.paste(im)

    # add image dimensions
    data += array.array("H", [bg.size[0], bg.size[1]]).tostring()

    # add color data
    for color in bg.getdata():
        data += array.array("H", [((color[0] >> 3) << 11) | ((color[1] >> 2) << 5) | (color[2] >> 3)]).tostring()

# write output
with open("icons.dat", "w") as fp:
    fp.write(data)