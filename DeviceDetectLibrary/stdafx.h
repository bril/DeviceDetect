// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <boost/bind.hpp>
#include <algorithm>
#include <objbase.h>
#include <atlbase.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <vector>
#include <stdio.h>

#include "Win32Types.h"
#include "Utilities.h"
#include "AutoCriticalSection.h"

#include "IDeviceWatcherObserver.h"
#include "ICollector.h"
#include "IDeviceChanged.h"

#include "NotifyWindow.h"
#include "ResultEnumerator.h"

#include "EnumDevices.h"
#include "EnumDisks.h"

