#pragma once

// Temporarily disable the unused-function warning for external third-party code
#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4505)
#elif defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
#endif

#include <simpleble/SimpleBLE.h>

#if defined(_MSC_VER)
    #pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif

class BleScanner 
{
public:
    BleScanner() = default;

    // Start a 5-second scan and print nearby devices
    void scanForDevices();
};