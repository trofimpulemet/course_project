#include "buffer.h"

Buffer::Buffer(unsigned short size)
{
    this->size     = size;
    this->quantity = 0;
    for (unsigned short i = 0; i < size; i++)
    {
        buffer.push_back({{0.0f, 0, 0}, true});
    }
}

void Buffer::push(const request_t &request, std::vector<Source *> & sources)
{
    if (isBufferFull())
    {
        std::deque<buffer_state_t>::iterator it = std::min_element(buffer.begin(),
                                                              buffer.end(),
                                                              []
                                                              (buffer_state_t a,
                                                               buffer_state_t b)
                                                               { return a.request.time
                                                                        < b.request.time; });
        sources[it->request.numberOfSource]->incRequestsFailed();

        it->request = request;
    }
    else
    {
        for(std::deque<buffer_state_t>::iterator it = buffer.begin();
            it != buffer.end();
            it++)
        {
            if (it->is_free)
            {
                it->request = request;
                it->is_free = false;
                quantity++;
                break;
            }
        }
    }
}

bool Buffer::isBufferFull()
{
    for(std::deque<buffer_state_t>::iterator it = buffer.begin();
        it != buffer.end();
        it++)
    {
        if (it->is_free)
        {
            return false;
        }
    }
    return true;
}

unsigned short Buffer::find(request_t request)
{
    unsigned short i = 0;
    for(std::deque<buffer_state_t>::iterator it = buffer.begin();
        it != buffer.end();
        it++, i++)
    {
        if (it->request.numberOfSource == request.numberOfSource
            && it->request.number == request.number)
        {
            return i;
        }
    }
}

buffer_state_t Buffer::getBufferPositionCurrentState(unsigned long long position)
{
    return buffer.at(position);
}

request_t Buffer::pop(std::vector<Source *> & sources, float time)
{
    std::deque<buffer_state_t>::iterator it = std::max_element(buffer.begin(),
                                                               buffer.end(),
                                                               []
                                                               (buffer_state_t a,
                                                                buffer_state_t b)
                                                               { return a.request.time
                                                                        < b.request.time; });
    request_t request = it->request;
    it->request.time           = 0.0f;
    it->request.numberOfSource = 0;
    it->request.number         = 0;
    it->is_free = true;
    quantity--;
    sources[request.numberOfSource]->incRequestsProcessed();
    sources[request.numberOfSource]->updAvgWaitTimeInBuffer(time - request.time);
    sources[request.numberOfSource]->mwinb_push(time - request.time);
    return request;
}

unsigned short Buffer::getFreePlace()
{
    unsigned short i = 0;
    for(std::deque<buffer_state_t>::iterator it = buffer.begin();
        it != buffer.end();
        it++, i++)
    {
        if (it->is_free)
        {
            return i;
        }
    }
}

request_t Buffer::getNextFailRequest()
{
    std::deque<buffer_state_t>::iterator it = std::min_element(buffer.begin(),
                                                          buffer.end(),
                                                          []
                                                          (buffer_state_t a,
                                                           buffer_state_t b)
                                                           { return a.request.time
                                                                    < b.request.time; });
    return it->request;
}

request_t Buffer::getNextProcessRequest()
{
    std::deque<buffer_state_t>::iterator it = std::max_element(buffer.begin(),
                                                          buffer.end(),
                                                          []
                                                          (buffer_state_t a,
                                                           buffer_state_t b)
                                                           { return a.request.time
                                                                    < b.request.time; });
    return it->request;
}


