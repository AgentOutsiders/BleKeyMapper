#include "BleRule.h"

// Constructor
BleRule::BleRule(std::vector<uint8_t> payload, int keyCode) 
    : expectedPayload(payload), targetKeyCode(keyCode) {}

// Method to check if the received frame matches what we expect
bool BleRule::match(const std::vector<uint8_t>& incomingData) const {
    return expectedPayload == incomingData;
}

int BleRule::getKeyCode() const {
    return targetKeyCode;
}