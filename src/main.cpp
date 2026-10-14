#include <iostream>
#include <vector>
#include "RuleManager.h"
#include "ConfigStorage.h"
#include "BleScanner.h"

int main() 
{
    std::cout << "[INFO] Starting BLE Universal Keystroke Mapper..." << std::endl;

    RuleManager engine;
    ConfigStorage storage;

    // Simulate a user saving a brand new button from their remote
    std::vector<uint8_t> newButtonPayload = {0x11, 0x55, 0xFF};
    storage.saveRuleToProfile("profiles", "codeGeneratedTest", "AA:BB:CC:DD:EE:FF", "Home Button", newButtonPayload, {29, 46});

    storage.loadProfilesFromDirectory("profiles", engine);

    std::vector<uint8_t> incomingFrame = {0x11, 0x55, 0xFF}; 
    auto result = engine.evaluate("AA:BB:CC:DD:EE:FF", incomingFrame);

    if (result.has_value()) 
    {
        std::cout << "[ACTION] Match found! Simulating keycodes: ";
        for (int code : result.value()) 
        {
            std::cout << code << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "[WARN] Unrecognized frame dropped." << std::endl;
    }

    BleScanner radar;
    radar.scanForDevices();

    return 0;
}