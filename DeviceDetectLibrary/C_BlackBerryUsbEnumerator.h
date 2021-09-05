#ifndef _C_BLACKBERRYUSBENUMERATOR_H_
#define _C_BLACKBERRYUSBENUMERATOR_H_

#include "C_AUsbEnumerator.h"

namespace DeviceDetectLibrary
{
    namespace Connection
    {
        class BlackBerryUsbEnumerator : public AUsbEnumerator
        {
            typedef AUsbEnumerator Base;
        public:
            BlackBerryUsbEnumerator(const NotifyWindow& window, ICollector& collector);
        };
    }
}

#endif _C_BLACKBERRYUSBENUMERATOR_H_