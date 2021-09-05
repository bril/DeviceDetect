#include "stdafx.h"
#include "Win_CriticalSection.h"

using namespace DeviceDetectLibrary;

CriticalSection::CriticalSection()
{
   ::InitializeCriticalSection(&criticalSection_);
}

CriticalSection::~CriticalSection()
{
   ::DeleteCriticalSection(&criticalSection_);
}

void CriticalSection::Enter()
{
   ::EnterCriticalSection(&criticalSection_);
}

void CriticalSection::Leave()
{
   ::LeaveCriticalSection(&criticalSection_);
}


