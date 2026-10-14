#pragma once
#include <vector>
#include <cstdint>

class BleRule 
{
private:
    // std::vector is a dynamic array (like an ArrayList)
    // An 8-bit integer, for reading hexadecimal
    std::vector<uint8_t> expectedPayload; 
    std::vector<int> targetKeycodes;

public:
    // Constructor
    BleRule(const std::vector<uint8_t>& payload, const std::vector<int>& keycodes);


    // Method to check if the received frame matches what we expect
    bool match(const std::vector<uint8_t>& incomingData) const;

    // Get the associated keyboard key
    std::vector<int> getKeyCodes() const;
};