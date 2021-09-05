// AutodetectConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
    LogMessage("");
    LogMessage("Start detecting...");
    
    TestDeviceWatcherObserver crier;
    DeviceDetectLibrary::DeviceWatcherPtr watcher;

    watcher.reset(new DeviceDetectLibrary::DeviceWatcher(&crier));

    watcher->Start();

    int i = 0;
    std::cin >> i;

    LogMessage("End detecting...");
	return 0;
}

