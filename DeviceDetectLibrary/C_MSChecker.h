#ifndef _C_MS_CHECKER_H_
#define _C_MS_CHECKER_H_

#include "Win_Thread.h"
#include "Win_ManualResetEvent.h"

#include <boost/shared_ptr.hpp>

namespace DeviceDetectLibrary
{
    namespace Connection
    {
        class MSChecker
        {
        public:
            MSChecker(ICollector& collector_, const DeviceInfo::DeviceId& devicePath);
            ~MSChecker();

            void Start();
            void Stop();

        private:

            static void ThreadFunction(void* state);
            void ThreadFunction();

        private:

            DeviceInfo::DeviceId devicePath_;
            ICollector& collector_; 

            Thread thread_;
            ManualResetEvent stopEvent_;

        };

        typedef boost::shared_ptr<MSChecker> MSCheckerPtr;
    }
}


#endif // _C_MS_CHECKER_H_
