#pragma once
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>
#include <cstdint>
#include "BleRule.h"

class RuleManager 
{
private:
    // Maps a device MAC address to its specific list of rules
    std::unordered_map<std::string, std::vector<BleRule>> deviceRules;

public:
    // Default constructor
    RuleManager() = default;

    // Associates a rule with a specific device MAC address
    void addRule(const std::string& macAddress, const BleRule& rule);

    // Checks if a profile has already registered this MAC address
    bool hasDevice(const std::string& macAddress) const;

    // Evaluates a frame only against the rules of the device that sent it
    std::optional<std::vector<int>> evaluate(
        const std::string& macAddress, 
        const std::vector<uint8_t>& frame
    ) const;

    // Returns the list of all known MAC addresses for the BLE scanner
    std::vector<std::string> getRegisteredMacs() const;
};