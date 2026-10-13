#pragma once

// Temporarily disable the unused-function warning for external third-party code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include <simpleble/SimpleBLE.h>
#pragma GCC diagnostic pop

class BleScanner 
{
public:
    BleScanner() = default;

    // Start a 5-second scan and print nearby devices
    void scanForDevices();
};