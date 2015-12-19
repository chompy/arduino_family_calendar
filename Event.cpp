#include "Event.h"

Event::Event(File eventFile)
{
    
}

Event::~Event()
{
    if (title) {
        delete title;
    }
}

Event Event::getNextEvent()
{
    return getNextEvent(day(), month(), year());
}

Event Event::getNextEvent(Event* event);
{
    return getNextEvent(
        event->getDay(),
        event->getMonth(),
        event->getYear()
    );
}

Event Event::getNextEvent(uint8_t _day, uint8_t _month, uint16_t _year)
{

}