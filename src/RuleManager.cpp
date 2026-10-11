#include "RuleManager.h"

void RuleManager::addRule(const BleRule& rule) 
{
    rules.push_back(rule);
}

std::optional<int> RuleManager::evaluate(const std::vector<uint8_t>& frame) const 
{
    for (const auto& rule : rules) 
    {
        if (rule.match(frame)) 
        {
            // If a rule matches, return the associated key code
            return rule.getKeyCode();
        }
    }
    // // If no rule matches, return std::nullopt to indicate "nothing"
    return std::nullopt; 
}