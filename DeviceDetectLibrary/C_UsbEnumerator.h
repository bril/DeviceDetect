#ifndef _C_USBENUMERATOR_H_
#define _C_USBENUMERATOR_H_

#include "ICollector.h"
#include "C_AUsbEnumerator.h"

namespace DeviceDetectLibrary
{
    class NotifyWindow;

    namespace Connection
    {
        class UsbEnumerator : public AUsbEnumerator
        {
            typedef AUsbEnumerator Base;
        public:
            UsbEnumerator(const NotifyWindow& window, ICollector& collector);
        };
    }
}


#endif //  _C_TESTENUMERATOR_H_
