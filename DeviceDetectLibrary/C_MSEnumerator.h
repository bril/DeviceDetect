#ifndef _C_MSENUMERATOR_H_
#define _C_MSENUMERATOR_H_

#include "IEnumerator.h"
#include "C_MSChecker.h"
#include "Win_CriticalSection.h"

namespace DeviceDetectLibrary
{
    namespace Connection
    {
        class MSEnumerator : public IEnumerator
        {
        public:
            MSEnumerator(ICollector& collector);
            ~MSEnumerator();

        public: // IEnumerator

            void Collect(const DeviceInfo& deviceInfo);

        public: 

            void TryThis(const DeviceInfo::DeviceId& devicePath);
            void RemoveThis(const DeviceInfo::DeviceId& devicePath);

        private:

            ICollector& collector_;

            typedef std::map<DeviceInfo::DeviceId, MSCheckerPtr> CheckersTable;
            CheckersTable checkers_;

            CriticalSection sec_;

        };

        typedef boost::shared_ptr<MSEnumerator> MSEnumeratorPtr;
    }
}


#endif //_C_MSENUMERATOR_H_
