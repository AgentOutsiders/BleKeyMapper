#pragma once
#include <string>
#include "RuleManager.h"

class ConfigStorage 
{
public:
    // Default constructor
    ConfigStorage() = default;

    // Scans the directory and injects found rules into the provided manager
    void loadProfilesFromDirectory(const std::string& directoryPath, RuleManager& manager);

    // Creates or updates a profile file with a new rule
    void saveRuleToProfile(
        const std::string& directoryPath, 
        const std::string& profileName, 
        const std::string& macAddress,
        const std::string& ruleName, 
        const std::vector<uint8_t>& payload, 
        const std::vector<int>& keycodes
    );
};