#include "device.h"

Device::Device(unsigned short number, float alpha, float beta)
{
    this->number = number;
    this->alpha  = alpha;
    this->beta   = beta;

    current_request = {0.0, 0, 0};
    releaseTime     = 0.0;
    employmentRate  = 0.0;

    processingTimeOfNextRequest = (float)qrand() / (float)RAND_MAX * (beta - alpha) + alpha;
    totalDeviceProcessingTime   = processingTimeOfNextRequest;
}

void Device::process(request_t request, std::vector<Source *> & sources, float time)
{
    current_request = request;

    sources[request.numberOfSource]->updAvgProcessingTime(processingTimeOfNextRequest);
    sources[request.numberOfSource]->mwond_push(processingTimeOfNextRequest);
    releaseTime = time + processingTimeOfNextRequest;

    processingTimeOfNextRequest = (float)qrand() / (float)RAND_MAX * (beta - alpha) + alpha;
    totalDeviceProcessingTime  += processingTimeOfNextRequest;
}

void Device::setEmploymentRate(float time)
{
    if (releaseTime == 0.0f)
    {
        employmentRate = 0.0f;
    }
    else
    {
        employmentRate = (totalDeviceProcessingTime - processingTimeOfNextRequest) / time * 100.0f;
    }
}
