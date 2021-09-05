#ifndef _ICOLLECTOR_H_
#define _ICOLLECTOR_H_

#include "IEnumerator.h"
#include "DeviceInfo.h"

namespace DeviceDetectLibrary
{
    class ICollector
    {
    public:
        virtual void Found(const DeviceInfo& diveceInfo) = 0;
        virtual void Lost(const DeviceInfo::DeviceId& diveceId) = 0;
        virtual ~ICollector() {}
    };
}

#endif // _ICOLLECTOR_H_