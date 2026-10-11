#pragma once
#include <vector>
#include <cstdint>

class BleRule 
{
private:
    // std::vector is a dynamic array (like an ArrayList)
    // An 8-bit integer, for reading hexadecimal
    std::vector<uint8_t> expectedPayload; 
    int targetKeyCode;

public:
    // Constructor
    BleRule(std::vector<uint8_t> payload, int keyCode);


    // Method to check if the received frame matches what we expect
    bool match(const std::vector<uint8_t>& incomingData) const;

    // Get the associated keyboard key
    int getKeyCode() const;
};