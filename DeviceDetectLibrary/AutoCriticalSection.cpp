#include "stdafx.h"
#include "AutoCriticalSection.h"

using namespace DeviceDetectLibrary;

AutoCriticalSection::AutoCriticalSection(CriticalSection& section)
: section_(section)
{
   section_.Enter();
}

AutoCriticalSection::~AutoCriticalSection() 
{
   section_.Leave();
}

