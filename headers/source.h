#ifndef SOURCE_H
#define SOURCE_H

#include "request.h"

#include <random>
#include <iostream>
#include <QDebug>
#include <QDateTime>
#include <cmath>

class Source
{
public:
    Source(unsigned short number, float lambda);
    request_t createNewRequest();

    void incRequestsProcessed() { processed++; }
    void incRequestsFailed()    { failed++;    }
    void incRequestsGiven()     { given++;     }

    unsigned short getNumber()           { return number; }
    unsigned long getRequestsProcessed() { return processed; }
    unsigned long getRequestsFailed()    { return failed;    }
    unsigned long getRequestsGiven()     { return given;     }

    float getTimeOfNextRequest()        { return timeOfNextRequest; }

    float getAvgWaitTimeInBuffer();
    float getAvgProcessingTime();

    float getAvgTimeInSystem();

    float getProbabilityOfFailure();

    void updAvgWaitTimeInBuffer(float time);
    void updAvgProcessingTime(float time);

    void mwinb_push(float time) { math_waiting_in_buffer.push_back(time); }
    void mwond_push(float time) { math_waiting_on_device.push_back(time); }

    float getDispersionOfTimeInBuffer();
    float getDispersionOfTimeOnDevice();

private:
    unsigned short number;
    float lambda;
    unsigned long processed,
                  failed,
                  given;
    float timeOfNextRequest;
    unsigned long numberOfCurrentRequest;
    float avgWaitTimeInBuffer,
          avgProcessingTime;
    std::vector<float> math_waiting_in_buffer;
    std::vector<float> math_waiting_on_device;
};


#endif // SOURCE_H
