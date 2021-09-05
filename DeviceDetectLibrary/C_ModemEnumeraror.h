#ifndef _C_MODEM_ENUMERATOR_H_
#define _C_MODEM_ENUMERATOR_H_

#include "C_ModemdeviceChecker.h"

namespace DeviceDetectLibrary
{
    namespace Connection
    {
        class ModemEnumeraror : public IEnumerator
        {
        public:
            ModemEnumeraror(ICollector& collector);
            ~ModemEnumeraror();

        public: // IEnumerator

            void Collect(const DeviceInfo& deviceInfo);            

        private:

            ICollector& collector_;
            typedef std::map<std::wstring, DeviceDetectLibrary::ModemDeviceNameParsers> Modems;
            
            Modems modems_;

        };
    }
}



#endif // _C_MODEM_ENUMERATOR_H_