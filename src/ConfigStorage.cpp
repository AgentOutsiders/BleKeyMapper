#include "ConfigStorage.h"
#include "toml.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

// Alias for convenience
namespace fs = std::filesystem;

void ConfigStorage::loadProfilesFromDirectory(const std::string& directoryPath, RuleManager& manager) 
{
    std::cout << "[INFO] Scanning for TOML profiles in: " << directoryPath << std::endl;

    // Check if directory exists
    if (!fs::exists(directoryPath)) 
    {
        std::cerr << "[WARN] Directory does not exist: " << directoryPath << std::endl;
        return;
    }

    // Iterate through directory
    for (const auto& entry : fs::directory_iterator(directoryPath)) 
    {
        if (entry.is_regular_file() && entry.path().extension() == ".toml") 
        {
            std::cout << "[INFO] Loading profile: " << entry.path().filename().string() << std::endl;
            
            try 
            {
                // Parse the TOML file
                toml::table config = toml::parse_file(entry.path().string());

                std::string macAddress = config["mac_address"].value_or("");
                if (macAddress.empty()) 
                {
                    std::cerr << "[WARN] Skipping profile with missing 'mac_address': " << entry.path().filename() << std::endl;
                    continue;
                }

                if (manager.hasDevice(macAddress)) 
                {
                    std::cerr << "[WARN] Duplicate MAC address (" << macAddress << ") in " 
                              << entry.path().filename() << ". Keeping first loaded profile, skipping this file." << std::endl;
                    continue;
                }

                toml::array* rulesArray = config.get_as<toml::array>("rules");

                if (rulesArray != nullptr) 
                {
                    for (auto& element : *rulesArray) 
                    {
                        toml::table* ruleTable = element.as_table();
                        if (ruleTable != nullptr) 
                        {
                            // Extract KeyCodes array
                            std::vector<int> keycodes;
                            toml::array* keycodesArray = ruleTable->get_as<toml::array>("keycodes");
                        
                            if (keycodesArray != nullptr) 
                            {
                                for (auto& codeElement : *keycodesArray) 
                                {
                                    if (codeElement.is_integer()) 
                                    {
                                        keycodes.push_back(static_cast<int>(codeElement.as_integer()->get()));
                                    }
                                }
                            }
                        
                            // Extract Payload array
                            std::vector<uint8_t> payload;
                            toml::array* payloadArray = ruleTable->get_as<toml::array>("payload");

                            if (payloadArray != nullptr) 
                            {
                                for (auto& byteElement : *payloadArray) 
                                {
                                    if (byteElement.is_integer()) 
                                    {
                                        payload.push_back(static_cast<uint8_t>(byteElement.as_integer()->get()));
                                    }
                                }
                            }
                        
                            // If both arrays are valid and non-empty, inject into RuleManager
                            if (!payload.empty() && !keycodes.empty()) 
                            {
                                BleRule newRule(payload, keycodes);
                                manager.addRule(macAddress, newRule);
                                std::cout << "[CONFIG] Successfully loaded rule -> " << keycodes.size() << " keycode(s)" << std::endl;
                            }
                        }
                    }
                }
            }
            catch (const toml::parse_error& err) 
            {
                std::cerr << "[ERROR] Parsing failed for " << entry.path().filename().string() << ":\n" << err << std::endl;
            }
        }
    }
}

void ConfigStorage::saveRuleToProfile(
    const std::string& directoryPath, 
    const std::string& profileName, 
    const std::string& macAddress,
    const std::string& ruleName, 
    const std::vector<uint8_t>& payload, 
    const std::vector<int>& keycodes)
{
    std::cout << "[INFO] Saving rule '" << ruleName << "' to profile: " << profileName << std::endl;

    // Ensure the profiles directory exists
    if (!fs::exists(directoryPath))
    {
        fs::create_directory(directoryPath);
    }

    std::string filePath = directoryPath + "/" + profileName + ".toml";
    toml::table config;

    // Parse the file if it exists so we can modify it
    if (fs::exists(filePath))
    {
        try
        {
            config = toml::parse_file(filePath);
        }
        catch (const toml::parse_error& err)
        {
            std::cerr << "[WARN] Could not parse existing file, starting fresh: \n" << err << std::endl;
        }
    }

    // Set or update the MAC address at the root of the TOML file
    config.insert_or_assign("mac_address", macAddress);

    // Build the payload array and force hexadecimal formatting
    toml::array payloadArray;
    for (uint8_t byte : payload)
    {
        toml::value<int64_t> hexValue(byte);
        hexValue.flags(toml::value_flags::format_as_hexadecimal);
        payloadArray.push_back(hexValue);
    }

    // Build the keycodes array
    toml::array keycodesArray;
    for (int code : keycodes)
    {
        keycodesArray.push_back(code);
    }

    toml::array* rulesArray = config.get_as<toml::array>("rules");
    bool ruleUpdated = false;

    if (rulesArray != nullptr)
    {
        // Search for an existing rule with the same name
        for (auto& element : *rulesArray)
        {
            toml::table* existingRule = element.as_table();
            
            if (existingRule != nullptr)
            {
                auto nameNode = existingRule->get("name");
                
                // If a rule with this name exists, update it
                if (nameNode != nullptr && nameNode->value_or("") == ruleName)
                {
                    existingRule->insert_or_assign("keycodes", keycodesArray);
                    existingRule->insert_or_assign("payload", payloadArray);
                    ruleUpdated = true;
                    break;
                }
            }
        }
    }

    // If the rule was not found, create a new one and append it
    if (!ruleUpdated)
    {
        toml::table newRule;
        newRule.insert("name", ruleName);
        newRule.insert("keycodes", keycodesArray);
        newRule.insert("payload", payloadArray);

        if (rulesArray == nullptr)
        {
            toml::array newRulesArray;
            newRulesArray.push_back(newRule);
            config.insert("rules", newRulesArray);
        }
        else
        {
            rulesArray->push_back(newRule);
        }
    }

    // Open the file and write the TOML content
    std::ofstream outputFile(filePath);
    if (outputFile.is_open())
    {
        outputFile << config << "\n";
        outputFile.close();
        std::cout << "[SUCCESS] Rule saved to " << filePath << std::endl;
    }
    else
    {
        std::cerr << "[ERROR] Could not open file for writing: " << filePath << std::endl;
    }
}