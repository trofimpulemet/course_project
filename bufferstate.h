#ifndef BUFFERSTATE_H
#define BUFFERSTATE_H

#include "request.h"

struct buffer_state_t
{
    request_t request;
    bool is_free;
};

#endif // BUFFERSTATE_H
