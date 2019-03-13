#include "source.h"

Source::Source(unsigned short number, float lambda)
{
    this->number           = number;
    this->lambda           = lambda;

    processed              = 0;
    failed                 = 0;
    given                  = 0;
    numberOfCurrentRequest = 0;
    avgWaitTimeInBuffer    = 0.0;
    avgProcessingTime      = 0.0;

    float uniform_random_number = 0.0f;
    while (uniform_random_number == 0.0f)
    {
        uniform_random_number = (float)qrand() / (float)RAND_MAX;
    }
    float test = std::log(uniform_random_number);
    timeOfNextRequest = -1.0f / this->lambda * test;
    if (timeOfNextRequest < 0.0f)
    {
        timeOfNextRequest *= -1.0f;
    }
}

request_t Source::createNewRequest()
{
    request_t request {getTimeOfNextRequest(), this->number, ++given};
    numberOfCurrentRequest = given;

    float uniform_random_number = 0.0f;
    while (uniform_random_number == 0.0f)
    {
        uniform_random_number = (float)qrand() / (float)RAND_MAX;
    }
    float test = std::log(uniform_random_number);
    float right_part = -1.0f / this->lambda * test;
    if (right_part < 0.0f)
    {
        right_part *= -1.0f;
    }
    timeOfNextRequest = timeOfNextRequest + right_part;
    return request;
}

void Source::updAvgWaitTimeInBuffer(float time)
{
    avgWaitTimeInBuffer += time;
}

void Source::updAvgProcessingTime(float time)
{
    avgProcessingTime += time;
}

float Source::getAvgWaitTimeInBuffer()
{
    if (processed == 0)
    {
        return 0.0f;
    }
    else
    {
        return avgWaitTimeInBuffer / processed;
    }
}

float Source::getAvgProcessingTime()
{
    if (processed == 0)
    {
        return 0.0f;
    }
    else
    {
        return avgProcessingTime / processed;
    }
}

float Source::getAvgTimeInSystem()
{
    return (avgWaitTimeInBuffer + avgProcessingTime) / processed;
}

float Source::getProbabilityOfFailure()
{
    return (float)getRequestsFailed() / (float)getRequestsGiven() * 100.0f;
}

float Source::getDispersionOfTimeInBuffer()
{
    float math_waiting = 0.0f;
    for (std::vector<float>::iterator it = math_waiting_in_buffer.begin();
         it != math_waiting_in_buffer.end();
         it++)
    {
        math_waiting += *it;
    }

    math_waiting /= math_waiting_in_buffer.size();

    float dispersion = 0.0f;
    for (std::vector<float>::iterator it = math_waiting_in_buffer.begin();
         it != math_waiting_in_buffer.end();
         it++)
    {
        dispersion += std::pow(*it - math_waiting, 2);
    }
    return dispersion / math_waiting_in_buffer.size();
}

float Source::getDispersionOfTimeOnDevice()
{
    float math_waiting = 0.0f;
    for (std::vector<float>::iterator it = math_waiting_on_device.begin();
         it != math_waiting_on_device.end();
         it++)
    {
        math_waiting += *it;
    }

    math_waiting /= math_waiting_on_device.size();

    float dispersion = 0.0f;
    for (std::vector<float>::iterator it = math_waiting_on_device.begin();
         it != math_waiting_on_device.end();
         it++)
    {
        dispersion += std::pow(*it - math_waiting, 2);
    }
    return dispersion / math_waiting_on_device.size();
}
