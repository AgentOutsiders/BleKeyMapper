#include "BleRule.h"

// Constructor
BleRule::BleRule(const std::vector<uint8_t>& payload, const std::vector<int>& keycodes) 
    : expectedPayload(payload), targetKeycodes(keycodes) {}

// Method to check if the received frame matches what we expect
bool BleRule::match(const std::vector<uint8_t>& incomingData) const 
{
    return expectedPayload == incomingData;
}

std::vector<int> BleRule::getKeyCodes() const 
{
    return targetKeycodes;
}