#include "StdAfx.h"
#include "DeviceInfo.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary;

DeviceInfo::DeviceInfo()
    : id_()
    , name_()
    , deviceDisplayName_(L"Unknown device")
{
}

DeviceInfo::DeviceInfo(const DeviceId& id, const std::wstring& name, std::wstring deviceDisplayName, const ConnectionInfo& connInfo) 
    : id_(Utilities::StringUpper(id))
    , name_(name)
    , deviceDisplayName_(deviceDisplayName)
    , connInfo_(connInfo)
{
}

DeviceInfo::~DeviceInfo()
{
}

