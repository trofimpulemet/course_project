#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "source.h"
#include "device.h"
#include "buffer.h"
#include "event.h"
#include "devicestate.h"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <iomanip>

class Controller
{
public:
    Controller(unsigned short numberOfSources,
               unsigned short numberOfDevices,
               unsigned short sizeOfBuffer,
               unsigned long  implementationLength,
               float alpha,
               float beta,
               float lambda,
               std::vector<int> priorities);
    ~Controller();

    float getTimeOfModeling() { return timeOfModeling; }

    bool isEndOfModeling(unsigned long implementationLength);
    bool isAnyDeviceFree(float appTime);

    int findSourceWithMinTimeOfNextRequest(unsigned long implementationLength);

    std::multimap<int, Device *>::iterator getFreeDevice(float timeOfLastRequest);

    std::multimap<int, Device *>::iterator getDeviceWithMaxReleaseTime();
    std::multimap<int, Device *>::iterator getDeviceWithMinReleaseTime();

    std::multimap<int, Device *> getDevices()        { return devices; }
    std::vector<Source *>        getSources()        { return sources; }

    void modeling();

    void printInformationOfModeling();

    std::vector<device_state_t> getDevicesState(float time);
    std::vector<std::pair<unsigned long,
                          unsigned long>> getSourcesState();
    std::vector<buffer_state_t> getBufferState();
    std::vector<event_t> getEvents() { return events; }

    unsigned short getSizeOfBuffer() { return sizeOfBuffer; }


private:
    std::vector<Source *> sources;
    std::multimap<int, Device *> devices;
    Buffer *buffer;
    float timeOfModeling;
    unsigned long implementationLength;
    std::vector<event_t> events;
    unsigned short sizeOfBuffer;
};

#endif // CONTROLLER_H
