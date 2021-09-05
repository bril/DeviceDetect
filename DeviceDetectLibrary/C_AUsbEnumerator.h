#ifndef _C_AUSBENUMERATOR_H_
#define _C_AUSBENUMERATOR_H_

#include <boost/shared_ptr.hpp>
#include <windows.h>
#include <string>
#include <map>

#include "IEnumerator.h"
#include "ICollector.h"

namespace DeviceDetectLibrary
{
    class NotifyWindow;

    namespace Connection
    {
        class AUsbEnumerator : public IEnumerator
        {
        public:
            AUsbEnumerator(ICollector& collector); 
            virtual ~AUsbEnumerator();

            virtual void TryThis(const GUID& guid); 
            virtual void AddCollector(const GUID& guid, IEnumeratorPtr enumerator, const NotifyWindow& window);

        public: // IEnumerator
            virtual void Collect(const DeviceInfo& deviceInfo);

        private:
            struct GTPair : boost::noncopyable
            {
            public:
                GTPair(const GUID& guid, IEnumeratorPtr enumerator, const NotifyWindow& window ); 
                ~GTPair(); 

            public:
                GUID Guid;
                IEnumeratorPtr Enumerator;

            private:
                HDEVNOTIFY devNotify_;
            };

            typedef boost::shared_ptr<GTPair> GTPairPtr;
            typedef std::vector<GTPairPtr> GuidTable;

            GuidTable table_;
            ICollector& collector_;
        };

        typedef boost::shared_ptr<AUsbEnumerator> UsbEnumeratorPtr;
    }
}

#endif _C_AUSBENUMERATOR_H_