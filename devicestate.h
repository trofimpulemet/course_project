#ifndef DEVICESTATE_H
#define DEVICESTATE_H

#include "request.h"

struct device_state_t
{
    request_t request;
    bool is_free;
    float releaseTime;
};

#endif // DEVICESTATE_H
