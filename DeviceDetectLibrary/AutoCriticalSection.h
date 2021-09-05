#ifndef _AUTO_CRITICAL_SECTION_H_
#define _AUTO_CRITICAL_SECTION_H_

#include "Win_CriticalSection.h"

namespace DeviceDetectLibrary
{
	class AutoCriticalSection
	{
	public:
		AutoCriticalSection(CriticalSection& section);
		virtual ~AutoCriticalSection();

	private:
		AutoCriticalSection(const AutoCriticalSection& section);
		AutoCriticalSection& operator=(const AutoCriticalSection& section);

    private:
    	CriticalSection& section_;
	};
} 

#endif // __AUTO_CRITICAL_SECTION_H_INCLUDED__

