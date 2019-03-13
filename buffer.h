#ifndef BUFFER_H
#define BUFFER_H

#include "bufferstate.h"
#include "source.h"
#include <utility>

#include <queue>
#include <iterator>
#include <algorithm>
#include <iostream>

class Buffer
{
public:
    Buffer(unsigned short size);

    void push(const request_t &request, std::vector<Source *> & sources);
    request_t pop(std::vector<Source *> & sources, float time);

    bool isBufferFull();
    buffer_state_t getBufferPositionCurrentState(unsigned long long position);

    unsigned short getCurrentQuantity() { return quantity; }
    unsigned short getBufferSize()      { return size;     }

    unsigned short find(request_t request);
    unsigned short getFreePlace();
    request_t getNextFailRequest();
    request_t getNextProcessRequest();

private:
    std::deque<buffer_state_t> buffer;
    unsigned short size;
    unsigned short quantity;
};


#endif // BUFFER_H
