#include "StdAfx.h"
#include "C_ModemEnumeraror.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

ModemEnumeraror::ModemEnumeraror(ICollector& collector)
    : collector_(collector)
{           
    std::vector<std::wstring> motorolaPattern;
    // for example connected mobile phone Motorola
    motorolaPattern.push_back(L"Motorola USB");
    ModemDeviceNameParsers motorolaParsers;
    motorolaParsers.push_back(ModemDeviceNameParserPtr(new ModemDeviceChecker(collector, motorolaPattern)));
    modems_[L"Motorola Modem device"] = motorolaParsers;
   
}

ModemEnumeraror::~ModemEnumeraror()
{
}

void ModemEnumeraror::Collect( const DeviceInfo& deviceInfo )
{    
    for (Modems::const_iterator iter = modems_.begin(); iter != modems_.end(); ++iter)
    {
        for (ModemDeviceNameParsers::const_iterator parserIterator = iter->second.begin(); parserIterator != iter->second.end(); ++parserIterator )
        {
            (*parserIterator)->Collect(deviceInfo, iter->first);            
        }       
    }
}
