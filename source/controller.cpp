#include "controller.h"

Controller::Controller(unsigned short numberOfSources,
                       unsigned short numberOfDevices,
                       unsigned short sizeOfBuffer,
                       unsigned long  implementationLength,
                       float alpha,
                       float beta,
                       float lambda,
                       std::vector<int> priorities):
timeOfModeling(0.0f),
implementationLength(implementationLength),
sizeOfBuffer(sizeOfBuffer)
{
    //qsrand(time(0));

    for (int i = 0; i < numberOfSources; i++)
    {
        sources.push_back(new Source(i, lambda));
    }

    int number = 0;
    for (std::vector<int>::iterator it = priorities.begin();
         it != priorities.end();
         it++, number++)
    {
        devices.insert(std::make_pair(*it, new Device(number, alpha, beta)));
    }

    buffer = new Buffer(sizeOfBuffer);
}

Controller::~Controller()
{
    for (std::vector<Source *>::iterator it = sources.begin();
         it != sources.end();
         it++)
    {
        delete *it;
    }

    for (std::multimap<int, Device *>::iterator it = devices.begin();
         it != devices.end();
         it++)
    {
        delete it->second;
    }

    delete buffer;
}

int Controller::findSourceWithMinTimeOfNextRequest(unsigned long implementationLength)
{
    int index = 0;
    float min = std::numeric_limits<float>::infinity();
    for (std::vector<Source *>::iterator it = sources.begin();
         it != sources.end();
         it++)
    {
        if ((*it)->getRequestsGiven() == implementationLength)
        {
            continue;
        }

        if ((*it)->getTimeOfNextRequest() <= min)
        {
            min = (*it)->getTimeOfNextRequest();
            index = (*it)->getNumber();
        }
    }
    return index;
}

bool Controller::isEndOfModeling(unsigned long implementationLength)
{
    for (std::vector<Source *>::iterator it = sources.begin();
         it != sources.end();
         it++)
    {
        if ((*it)->getRequestsGiven() != implementationLength)
        {
            return false;
        }
    }
    return true;
}

bool Controller::isAnyDeviceFree(float appTime)
{
    for (std::multimap<int, Device *>::iterator it = devices.begin();
         it != devices.end();
         it++)
    {
        if (it->second->getReleaseTime() <= appTime)
        {
            return true;
        }
    }
    return false;
}

void Controller::modeling()
{
    request_t request {0.0, 0, 0};
    while (!isEndOfModeling(implementationLength))
    {
        int number_of_source = findSourceWithMinTimeOfNextRequest(implementationLength);
        request = sources[number_of_source]->createNewRequest();
        timeOfModeling = request.time;
        event_t event = {timeOfModeling,
                         "Заявка " + QString::number(request.numberOfSource) +
                         "." + QString::number(request.number) + " поступила в систему",
                         getDevicesState(timeOfModeling),
                         getSourcesState(),
                         getBufferState()};
        events.push_back(event);
        if (!buffer->isBufferFull())
        {
            unsigned short free_place = buffer->getFreePlace();
            buffer->push(request, sources);

            event = {timeOfModeling,
                     "Заявка " + QString::number(request.numberOfSource) +
                     "." + QString::number(request.number) + " загружена "
                     "в буфер на позицию " + QString::number(free_place),
                     getDevicesState(timeOfModeling),
                     getSourcesState(),
                     getBufferState()};
            events.push_back(event);
        }
        else
        {
            request_t fail_request = buffer->getNextFailRequest();

            buffer->push(request, sources);

            event = {timeOfModeling,
                     "Заявка " + QString::number(request.numberOfSource) +
                     "." + QString::number(request.number) + " выбивает "
                     "из буфера с позиции " + QString::number(buffer->find(request)) +
                     " заявку " + QString::number(fail_request.numberOfSource) +
                     "." + QString::number(fail_request.number),
                     getDevicesState(timeOfModeling),
                     getSourcesState(),
                     getBufferState()};
            events.push_back(event);
        }

        while (buffer->getCurrentQuantity() > 0
               && isAnyDeviceFree(timeOfModeling))
        {
            request_t next_pop_request = buffer->getNextProcessRequest();
            unsigned short pos_of_next_pop_request = buffer->find(next_pop_request);

            request = buffer->pop(sources, timeOfModeling);
            std::multimap<int, Device *>::iterator free_device = getFreeDevice(timeOfModeling);
            free_device->second->process(request, sources, timeOfModeling);
            event = {timeOfModeling,
                     "Заявка " + QString::number(request.numberOfSource) +
                     "." + QString::number(request.number) + " выгружена "
                     "из буфера с позиции " + QString::number(pos_of_next_pop_request) +
                     " и будет обработана прибором " + QString::number(free_device->second->getNumber()),
                     getDevicesState(timeOfModeling),
                     getSourcesState(),
                     getBufferState()};
            events.push_back(event);
        }
    }

    while(buffer->getCurrentQuantity() > 0)
    {
        request_t next_pop_request = buffer->getNextProcessRequest();
        unsigned short pos_of_next_pop_request = buffer->find(next_pop_request);

        std::multimap<int, Device *>::iterator it = getDeviceWithMinReleaseTime();
        timeOfModeling = it->second->getReleaseTime();
        request = buffer->pop(sources, timeOfModeling);
        it->second->process(request, sources, timeOfModeling);

        event_t event = {timeOfModeling,
                 "Заявка " + QString::number(request.numberOfSource) +
                 "." + QString::number(request.number) + " выгружена "
                 "из буфера с позиции " + QString::number(pos_of_next_pop_request) +
                 " и будет обработана прибором " + QString::number(it->second->getNumber()),
                 getDevicesState(timeOfModeling),
                 getSourcesState(),
                 getBufferState()};
        events.push_back(event);
    }
    std::multimap<int, Device *>::iterator it = getDeviceWithMaxReleaseTime();
    timeOfModeling = it->second->getReleaseTime();

    for (std::multimap<int, Device *>::iterator it = devices.begin();
         it != devices.end();
         it++)
    {
        it->second->setEmploymentRate(timeOfModeling);
    }

}

std::multimap<int, Device *>::iterator Controller::getFreeDevice(float timeOfLastRequest)
{
    for (std::multimap<int, Device *>::iterator it = devices.begin();
         it != devices.end();
         it++)
    {
        if (it->second->getReleaseTime() <= timeOfLastRequest)
        {
            return it;
        }
    }
}

std::multimap<int, Device *>::iterator Controller::getDeviceWithMaxReleaseTime()
{
    return std::max_element(devices.begin(),
                            devices.end(),
                            []
                            (std::pair<int, Device *> a,
                             std::pair<int, Device *> b)
                            { return a.second->getReleaseTime()
                                     < b.second->getReleaseTime(); });
}

std::multimap<int, Device *>::iterator Controller::getDeviceWithMinReleaseTime()
{
    return std::min_element(devices.begin(),
                            devices.end(),
                            []
                            (std::pair<int, Device *> a,
                             std::pair<int, Device *> b)
                            { return a.second->getReleaseTime()
                                     < b.second->getReleaseTime(); });
}

std::vector<device_state_t> Controller::getDevicesState(float time)
{
    std::vector<device_state_t> states;
    for (std::multimap<int, Device *>::iterator it = devices.begin();
         it != devices.end();
         it++)
    {
        if (it->second->getReleaseTime() > time)
        {
            states.push_back({it->second->getCurrentRequest(), false, it->second->getReleaseTime()});
        }
        else
        {
            states.push_back({{0.0f, 0, 0}, true, it->second->getReleaseTime()});
        }
    }
    return states;
}

std::vector<std::pair<unsigned long,
                      unsigned long>> Controller::getSourcesState()
{
    std::vector<std::pair<unsigned long,
                          unsigned long>> states;
    for (std::vector<Source *>::iterator it = sources.begin();
         it != sources.end();
         it++)
    {
        states.push_back(std::make_pair((*it)->getRequestsGiven(), (*it)->getRequestsFailed()));
    }
    return states;
}

std::vector<buffer_state_t> Controller::getBufferState()
{
    std::vector<buffer_state_t> states;
    for (int i = 0; i < buffer->getBufferSize(); i++)
    {
        states.push_back(buffer->getBufferPositionCurrentState(i));
    }
    return states;
}

void Controller::printInformationOfModeling()
{
    std::cout << "Main time: "
              << timeOfModeling << "; buffer quantity on end of modeling: "
              << buffer->getCurrentQuantity() << "\n";
    for (int i = 0; i < buffer->getBufferSize(); i++)
    {
        std::cout << "position " << i << ": ";
        if (buffer->getBufferPositionCurrentState(i).is_free == true)
        {
            std::cout << "free\n";
        }
        else
        {
            std::cout << "busy\n";
        }
    }
    std::cout << "\n";

    int i = 0;
    for (std::vector<Source *>::iterator it = sources.begin();
         it != sources.end();
         it++, i++)
    {
        std::cout << "Source: " << i << ": \n"

                  << "given: "     << (*it)->getRequestsGiven()        << "; "
                  << "processed: " << (*it)->getRequestsProcessed()    << "; "
                  << "failed: "    << (*it)->getRequestsFailed()       << "; "
                  << "PoF: "       << (*it)->getProbabilityOfFailure() << "%; "
                  << "ATinB: "     << (*it)->getAvgWaitTimeInBuffer()  << "; "
                  << "ATofP: "     << (*it)->getAvgProcessingTime()    << "; "
                  << "ATinSys: "   << (*it)->getAvgTimeInSystem()      << "; "
                  // dispersions
                  << std::endl;
    }
    std::cout << "\n";

    for (std::multimap<int, Device *>::iterator it = devices.begin();
         it != devices.end();
         it++)
    {
        std::cout << "Device "
                  << it->second->getNumber()
                  << ": employment rate = "
                  << std::fixed << std::setprecision(4)
                  << it->second->getEmploymentRate()
                  << "%\n";

    }
}
