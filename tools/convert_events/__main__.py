import sys
import os
import array
import ConfigParser
import StringIO
import datetime
import dateutil.rrule

# define constant vars
EVENT_PATH = os.path.join( os.path.dirname(os.path.realpath(__file__)), "events" )
TODAY = datetime.date.today()
MAX_YEARS = 50
EVENT_TYPES = {
    "bday" : 1,
    "holiday" : 2,
    "anniversary" : 3,
    "event" : 4
}

# make sure event dir exists
if not os.path.exists(EVENT_PATH) or not os.path.isdir(EVENT_PATH):
    print "Event directory not found."
    sys.exit()

# define vars
eventData = []
dateList = []
counter = 0

# iterate all event files
for filename in os.listdir(EVENT_PATH):
    fullpath = os.path.join( EVENT_PATH, filename )
    
    # open event file and make it structed like 
    # a proper INI file
    with open(fullpath, "r") as fp:
        eventFileString = "[root]\n" + fp.read()
    eventFp = StringIO.StringIO(eventFileString)
    eventFile = ConfigParser.RawConfigParser()
    eventFile.readfp(eventFp)
    eventFp.close()

    # build date
    eventDate = datetime.date(
        eventFile.getint("root", "year"),
        eventFile.getint("root", "month"),
        eventFile.getint("root", "day")
    )

    # convert event type to an int
    eventType = 0
    if ( eventFile.get("root", "type") in EVENT_TYPES ):
        eventType = EVENT_TYPES[ eventFile.get("root", "type") ]

    # build event data list
    eventData.append({
        "id" : counter,
        "title" : eventFile.get("root", "title"),
        "date" : eventDate,
        "type" : eventType,
    })

    # generate date list
    rrule = dateutil.rrule.rrulestr(eventFile.get("root", "rrule"), dtstart=eventDate)
    for date in rrule:
        if date.year < TODAY.year:
            continue
        elif date.year > TODAY.year + MAX_YEARS:
            break
        dateList.append([counter, date])

    counter += 1

# sort event dates
def sortDates(a, b):
    if (a[1].year != b[1].year):
        return a[1].year - b[1].year
    elif a[1].month != b[1].month:
        return a[1].month - b[1].month
    return a[1].day - b[1].day
dateList = sorted(dateList, cmp=sortDates)

# build output
data = ""

data += array.array("B", [len(eventData)]).tostring()
for event in eventData:
    data += array.array("B", [event["id"], event["type"]]).tostring()
    data += event["title"][:24].ljust(24, chr(0))

for date in dateList:
    data += array.array("B", [date[0], date[1].month, date[1].day]).tostring()
    data += array.array("H", [date[1].year]).tostring()

# write output
with open("events.dat", "w") as fp:
    fp.write(data)