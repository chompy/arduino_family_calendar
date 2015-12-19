#ifndef __CAL_EVENTS__
#define __CAL_EVENTS__

#include <Time.h>
#include <string.h>
#include "SPI.h"
#include <SD.h>

struct CalendarDate
{
    uint8_t month;
    uint8_t day;
    uint16_t year;
};

class Event
{
public:

    static const uint8_t PROGMEM TYPE_NONE = 0;
    static const uint8_t PROGMEM TYPE_BDAY = 1;
    static const uint8_t PROGMEM TYPE_HOLIDAY = 2;
    static const uint8_t PROGMEM TYPE_ANNIVERSARY = 3;
    static const uint8_t PROGMEM TYPE_EVENT = 4;

    static const uint8_t PROGMEM RULE_NONE = 0;
    static const uint8_t PROGMEM RULE_YEARLY = 1;
    static const uint8_t PROGMEM RULE_EASTER = 2;

    Event(File eventFile);
    ~Event();

    char* getTitle() { return title; }
    uint8_t getType() { return type; }
    uint8_t getMonth() { return month; }
    uint8_t getDay() { return day; }
    uint16_t getYear() { return year; }
    uint8_t getRule() { return rule; }

    CalendarDate getNextDate();
    CalendarDate getNextDate(CalendarDate* date);
    CalendarDate getNextDate(uint8_t _day, uint8_t _month, uint16_t _year);

    static Event getNextEvent();
    static Event getNextEvent(Event* event);
    static Event getNextEvent(uint8_t _day, uint8_t _month, uint16_t _year);

private:

    char* title;
    uint8_t type;
    uint8_t month;
    uint8_t day;
    uint16_t year;
    uint8_t rule;

};

#endif