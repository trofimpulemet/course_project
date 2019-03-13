#ifndef DEVICE_H
#define DEVICE_H

#include "request.h"
#include "source.h"

#include <iostream>
#include <QDebug>

class Device
{
public:
    Device(unsigned short number, float alpha, float beta);
    void process(request_t request, std::vector<Source *> & sources, float time);
    float getReleaseTime()                 { return releaseTime;                   }
    float getProcessingTimeOfNextRequest() { return processingTimeOfNextRequest;   }
    float getTotalDeviceProcessingTime()   { return totalDeviceProcessingTime
                                                    - processingTimeOfNextRequest; }

    void setEmploymentRate(float time);
    unsigned short getNumber() {return number; }
    request_t getCurrentRequest() { return current_request; }

    float getEmploymentRate() { return employmentRate; }

private:
    unsigned short number;
    float releaseTime,
          processingTimeOfNextRequest,
          totalDeviceProcessingTime;
    float alpha, beta;
    float employmentRate;
    request_t current_request;
};

#endif // DEVICE_H
