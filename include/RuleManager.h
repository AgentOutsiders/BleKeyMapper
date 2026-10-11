#pragma once
#include <vector>
#include <optional>
#include "BleRule.h"

class RuleManager 
{
private:
    std::vector<BleRule> rules;

public:
    // Default constructor
    RuleManager() = default;

    // Add a new rule to the manager
    void addRule(const BleRule& rule);

    // Evaluate a frame. Returns an int if found, or "nothing" if unknown.
    std::optional<int> evaluate(const std::vector<uint8_t>& frame) const;
};