#ifndef _RESULT_ENUMERATOR_H_
#define _RESULT_ENUMERATOR_H_

#include "IEnumerator.h"

namespace DeviceDetectLibrary
{
    class ResultEnumerator : public IEnumerator
    {
    public:
        ResultEnumerator(ICollector& collector, std::wstring deviceDisplayName, const std::wstring& friendlyName);
        ~ResultEnumerator();
    public:

        void Collect(const DeviceInfo& deviceInfo);

    private:
        std::wstring deviceID_;
        std::wstring friendlyName_;
        ICollector& collector_; 
    };
}



#endif // _RESULT_ENUMERATOR_H_