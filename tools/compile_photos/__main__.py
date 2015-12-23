import sys
import os
import array
from PIL import Image

MAX_PHOTO_W = 200
MAX_PHOTO_H = 240

# define constant vars
PHOTO_PATH = os.path.join( os.path.dirname(os.path.realpath(__file__)), "photos" )

# make sure photo dir exists
if not os.path.exists(PHOTO_PATH) or not os.path.isdir(PHOTO_PATH):
    print "Photo directory not found."
    sys.exit()


# collect data
data = ""

# get number of photos
data += array.array("B", [len( os.listdir(PHOTO_PATH) )]).tostring()

# iterate all photos
for filename in os.listdir(PHOTO_PATH):
    
    fullpath = os.path.join( PHOTO_PATH, filename )

    # open photo
    im = Image.open(fullpath)

    # resize
    if im.size[0] > MAX_PHOTO_W or im.size[1] > MAX_PHOTO_H:
        im.thumbnail((MAX_PHOTO_W, MAX_PHOTO_H))

    # add image dimensions
    data += array.array("H", [im.size[0], im.size[1]]).tostring()

    # add color data
    for color in im.getdata():
        data += array.array("H", [((color[0] >> 3) << 11) | ((color[1] >> 2) << 5) | (color[2] >> 3)]).tostring()

# write output
with open("photos.dat", "w") as fp:
    fp.write(data)