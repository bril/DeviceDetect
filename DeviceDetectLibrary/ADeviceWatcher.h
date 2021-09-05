#ifndef _A_DEVICE_WATCHER_H_
#define _A_DEVICE_WATCHER_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "Win_CriticalSection.h"
#include "IDeviceChanged.h"
#include "ICollector.h"
#include "DeviceInfo.h"
#include "NotifyWindow.h"

namespace DeviceDetectLibrary
{
    struct IDeviceWatcherObserver;

    class ADeviceWatcher : public boost::noncopyable, public ICollector, public IDeviceChanged
    {
    public:
        ADeviceWatcher(IDeviceWatcherObserver* observer);
        ~ADeviceWatcher();

        void Start();
        void Stop();

    public: // ICollector
        void Found(const DeviceInfo& deviceInfo);
        void Lost(const DeviceInfo::DeviceId& devId);

    protected:
        virtual void StartEnumerators() = 0;
        virtual void StopEnumerators() = 0;

        NotifyWindow& CreateNotifyWindow();
        void RemoveNotifyWindow();

    private:
        Devices actualDevices_;
        Devices tempDevices_;

        IDeviceWatcherObserver* observer_;

        CriticalSection section_;
        std::auto_ptr<NotifyWindow> window_;
    };

    typedef boost::shared_ptr<ADeviceWatcher> DeviceWatcherPtr;
}
#endif _A_DEVICE_WATCHER_H_