// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "BrightnessController.h"

#include "../Monitor.h"
#include "../Logger.h"

BrightnessController::BrightnessController(HMONITOR monitor) {
    BOOL result;
    DWORD numPhysicalMonitors = 0;
    result = GetNumberOfPhysicalMonitorsFromHMONITOR(
        monitor, &numPhysicalMonitors);

    if (result == FALSE || numPhysicalMonitors <= 0) {
        CLOG(L"Could not get physical monitors");
        return;
    }

    CLOG(L"Number of physical monitors detected: %d", numPhysicalMonitors);
    PHYSICAL_MONITOR *monitors = new PHYSICAL_MONITOR[numPhysicalMonitors];
    result = GetPhysicalMonitorsFromHMONITOR(
        monitor, numPhysicalMonitors, monitors);
    for (unsigned int i = 0; i < numPhysicalMonitors; ++i) {
        CLOG(L"Monitor: %s", monitors[i].szPhysicalMonitorDescription);
        bool supportsAPI = SupportsBrightnessAPI(monitors[i]);
        QCLOG(L"Supports *MonitorBrightness APIs: %s",
            supportsAPI ? L"YES" : L"NO");
        if (supportsAPI) {
            /* For now, we use the first compatible monitor found. */
            _monitorHandle = monitors[i].hPhysicalMonitor;
            break;
        }
    }
    delete[] monitors;

    DWORD min, cur, max;
    result = GetMonitorBrightness(_monitorHandle, &min, &cur, &max);
    if (result == 0) {
        Logger::LogLastError();
    }
    _minBrightness = min;
    _maxBrightness = max;
    CLOG(L"Got brightness: %d, %d, %d %f", min, cur, max, Brightness());
}

BrightnessController::BrightnessController(Monitor &monitor) :
BrightnessController(monitor.Handle()) {

}

float BrightnessController::Brightness() {
    DWORD cur;
    GetMonitorBrightness(_monitorHandle, NULL, &cur, NULL);
    return (float) (cur - _minBrightness) / (_maxBrightness - _minBrightness);
}

void BrightnessController::Brightness(float level) {

}

bool BrightnessController::SupportsBrightnessAPI(PHYSICAL_MONITOR &pm) {
    DWORD caps, color;
    BOOL result = GetMonitorCapabilities(pm.hPhysicalMonitor, &caps, &color);
    if (result == FALSE) {
        QCLOG(L"Monitor does not support DDC/CI");
        return false;
    }
    return ((caps & MC_CAPS_BRIGHTNESS) == MC_CAPS_BRIGHTNESS);
}
