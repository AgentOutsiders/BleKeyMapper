#include "RuleManager.h"

void RuleManager::addRule(const std::string& macAddress, const BleRule& rule) 
{
    // Automatically creates the vector if the MAC address is not registered yet
    deviceRules[macAddress].push_back(rule);
}

std::optional<int> RuleManager::evaluate(
    const std::string& macAddress, 
    const std::vector<uint8_t>& frame) const 
{
    // Find the specific device in memory
    auto it = deviceRules.find(macAddress);
    if (it == deviceRules.end()) 
    {
        return std::nullopt;
    }

    // Evaluate only the rules belonging to this device
    for (const auto& rule : it->second) 
    {
        if (rule.match(frame)) 
        {
            return rule.getKeyCode();
        }
    }

    return std::nullopt; 
}

std::vector<std::string> RuleManager::getRegisteredMacs() const 
{
    std::vector<std::string> macs;
    macs.reserve(deviceRules.size());

    for (const auto& pair : deviceRules) 
    {
        macs.push_back(pair.first);
    }

    return macs;
}