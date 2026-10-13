#include <iostream>
#include <vector>
#include "RuleManager.h"
#include "ConfigLoader.h"
#include "BleScanner.h"

int main() 
{
    std::cout << "[INFO] Starting BLE Universal Keystroke Mapper..." << std::endl;

    RuleManager engine;
    ConfigLoader loader;

    // Simulate a user saving a brand new button from their remote
    std::vector<uint8_t> newButtonPayload = {0x11, 0x55, 0xFF};
    loader.saveRuleToProfile("profiles", "codeGeneratedTest", "Home Button", newButtonPayload, 36);

    // Load all profiles from the "profiles" folder directly into our engine
    loader.loadProfilesFromDirectory("profiles", engine);

    // Fake incoming BLE frame for testing
    std::vector<uint8_t> incomingFrame = {0x11, 0x55, 0xFF}; 
    std::cout << "[INFO] Received frame from OS..." << std::endl;

    auto result = engine.evaluate("AA:BB:CC:DD:EE:FF", incomingFrame);
    
    if (result.has_value()) 
    {
        std::cout << "[ACTION] Match found! Simulating keycode: " << result.value() << std::endl;
    } 
    else
    {
        std::cout << "[WARN] Unrecognized frame dropped." << std::endl;
    }

    BleScanner radar;
    radar.scanForDevices();

    return 0;
}