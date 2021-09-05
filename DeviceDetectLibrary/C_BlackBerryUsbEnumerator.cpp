#include "StdAfx.h"
#include "C_BlackBerryUsbEnumerator.h"

using namespace DeviceDetectLibrary::Connection;
using namespace DeviceDetectLibrary;

BlackBerryUsbEnumerator::BlackBerryUsbEnumerator(const DeviceDetectLibrary::NotifyWindow &window, DeviceDetectLibrary::ICollector &collector)
    : Base(collector)
{
    GUID guidBB = { 0x80375827, 0x83b8, 0x4a51, { 0xb3, 0x9b, 0x90, 0x5f, 0xed, 0xd4, 0xf1, 0x18 } };
    IEnumeratorPtr enumeratorBB(new ResultEnumerator(collector, L"BlackBerry", L"BlackBerry USB Device"));
    AddCollector(guidBB, enumeratorBB, window);
}