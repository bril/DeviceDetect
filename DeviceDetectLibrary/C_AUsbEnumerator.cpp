#include "StdAfx.h"
#include "C_AUsbEnumerator.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;
using namespace DeviceDetectLibrary;

AUsbEnumerator::AUsbEnumerator(ICollector& collector)
:  collector_(collector)
{
}

AUsbEnumerator::~AUsbEnumerator()
{
}

void AUsbEnumerator::AddCollector(const GUID& guid, IEnumeratorPtr enumerator, const NotifyWindow& window)
{
    table_.push_back(GTPairPtr(new GTPair(guid, enumerator, window)));
}

void AUsbEnumerator::TryThis(const GUID& guid)
{
    struct GuidEqual
    {
        GuidEqual(const GUID& guid) : guid_(guid) {}

        bool operator()(const GTPairPtr& item) const
        {
            return memcmp(&(item->Guid), &guid_, sizeof(guid_)) == 0;
        }

    private:

        const GUID& guid_;
    };

    try
    {
        GuidTable::const_iterator guidIter = std::find_if(table_.begin(), table_.end(), GuidEqual(guid));
        if (guidIter != table_.end())
        {
            ConnectionInfo_vt result = GetDevicesByGuid(guid);
            for (ConnectionInfo_vt::const_iterator iter = result.begin(); iter != result.end(); ++iter)
            {
                (*guidIter)->Enumerator->Collect(DeviceInfo(iter->DevicePath, iter->FriendlyName, L"Unknown device", *iter));
            }
        }
    }
    catch (const std::exception&)
    {
        // ERROR: in detection
    }
}


void AUsbEnumerator::Collect(const DeviceInfo& deviceInfo) 
{
    struct Caller 
    {
        Caller(ICollector& collector, AUsbEnumerator& enumerator) 
            : collector_(collector) 
            , enumerator_(enumerator)
        {}

        void operator() (const GuidTable::const_iterator::value_type& pair)
        {
            enumerator_.TryThis(pair->Guid);
        }

    private:
        ICollector& collector_;
        AUsbEnumerator& enumerator_;
    };

    for_each(table_.begin(), table_.end(), Caller(collector_, *this));
}


AUsbEnumerator::GTPair::GTPair(const GUID& guid, IEnumeratorPtr enumerator, const NotifyWindow& window)
: Guid(guid)
, Enumerator(enumerator)
, devNotify_(RegisterUSBNotify(guid, window.GetHWND()))
{
}

AUsbEnumerator::GTPair::~GTPair()
{
    try
    {
        UnRegisterUSBNotify(devNotify_);
    }
    catch (const std::exception& /*e*/)
    {
        // ERROR: GTPair::~GTPair fails (%s)", e.what());
    }
}
