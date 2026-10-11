#include "ConfigLoader.h"
#include "toml.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

// Alias for convenience
namespace fs = std::filesystem;

void ConfigLoader::loadProfilesFromDirectory(const std::string& directoryPath, RuleManager& manager) 
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
                // toml::table is a map
                toml::table config = toml::parse_file(entry.path().string());
                toml::array* rulesArray = config.get_as<toml::array>("rules");

                if (rulesArray != nullptr) 
                {
                    for (auto& element : *rulesArray) 
                    {
                        toml::table* ruleTable = element.as_table();
                        if (ruleTable != nullptr) 
                        {
                            // Extract KeyCode
                            int keycode = ruleTable->at("keycode").value_or(-1);
                            
                            // Extract Payload array
                            std::vector<uint8_t> payload;
                            // toml::array is a std::vector wrapper who herits from toml::node (for the map/table), so we can use it like a vector
                            toml::array* payloadArray = ruleTable->get_as<toml::array>("payload");
                            
                            if (payloadArray != nullptr) 
                            {
                                for (auto& byteElement : *payloadArray) 
                                {
                                    if (byteElement.is_integer()) 
                                    {
                                        // Cast the 64-bit int from TOML to our 8-bit unsigned int
                                        payload.push_back(static_cast<uint8_t>(byteElement.as_integer()->get()));
                                    }
                                }
                            }

                            // If valid, inject into the RuleManager
                            if (!payload.empty() && keycode != -1) 
                            {
                                manager.addRule(BleRule(payload, keycode));
                                std::cout << "[CONFIG] Successfully loaded rule -> Keycode: " << keycode << std::endl;
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

void ConfigLoader::saveRuleToProfile(
    const std::string& directoryPath, 
    const std::string& profileName, 
    const std::string& ruleName, 
    const std::vector<uint8_t>& payload, 
    int keycode)
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

    // Build the payload array and force hexadecimal formatting
    toml::array payloadArray;
    for (uint8_t byte : payload)
    {
        toml::value<int64_t> hexValue(byte);
        hexValue.flags(toml::value_flags::format_as_hexadecimal);
        payloadArray.push_back(hexValue);
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
                    existingRule->insert_or_assign("keycode", keycode);
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
        newRule.insert("keycode", keycode);
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