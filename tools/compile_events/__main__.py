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
import ConfigParser
import StringIO
import datetime
import dateutil.rrule

# define constant vars
EVENT_PATH = os.path.join( os.path.dirname(os.path.realpath(__file__)), "events" )
TODAY = datetime.date.today()
MAX_YEARS = 150
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

# calculate easter
def calc_easter(year):
    "Returns Easter as a date object."
    a = year % 19
    b = year // 100
    c = year % 100
    d = (19 * a + b - b // 4 - ((b - (b + 8) // 25 + 1) // 3) + 15) % 30
    e = (32 + 2 * (b % 4) + 2 * (c // 4) - d - (c % 4)) % 7
    f = d + e - 7 * ((a + 11 * d + 22 * e) // 451) + 114
    month = f // 31
    day = f % 31 + 1    
    return datetime.date(year, month, day)

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

    # easter
    isEaster = False
    if eventFile.get("root", "type") == "holiday_easter":
        eventType = 2
        isEaster = True
    
    # convert event type to an int if not easter
    else :
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
    if not isEaster:
        rrule = dateutil.rrule.rrulestr(eventFile.get("root", "rrule"), dtstart=eventDate)
        for date in rrule:
            if date.year < TODAY.year:
                continue
            elif date.year > TODAY.year + MAX_YEARS:
                break
            dateList.append([counter, date])
    else:
        for year in range(TODAY.year, TODAY.year + MAX_YEARS):
            dateList.append([counter, calc_easter(year)])

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