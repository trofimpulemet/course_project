#ifndef EVENT_H
#define EVENT_H

#include "bufferstate.h"
#include "devicestate.h"

#include <QString>
#include <vector>

struct event_t
{
    float time;
    QString info;
    std::vector<device_state_t> devices_state;
    std::vector<std::pair<unsigned long,
                          unsigned long>> sources_state;
    std::vector<buffer_state_t> buffer_state;
};

#endif // EVENT_H
