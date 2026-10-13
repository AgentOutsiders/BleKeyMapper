#include "BleScanner.h"
#include <iostream>

void BleScanner::scanForDevices() 
{
    // Check if the OS has Bluetooth enabled
    if (!SimpleBLE::Adapter::bluetooth_enabled()) 
    {
        std::cerr << "[ERROR] Bluetooth is not enabled or not found on this machine." << std::endl;
        return;
    }

    // Get the list of physical antennas
    auto adapters = SimpleBLE::Adapter::get_adapters();
    if (adapters.empty()) 
    {
        std::cerr << "[ERROR] No Bluetooth adapters found." << std::endl;
        return;
    }

    // Iterate through every available adapter
    for (auto& adapter : adapters) 
    {
        std::cout << "\n[INFO] --- Initializing adapter: " << adapter.identifier() 
                  << " [" << adapter.address() << "] ---" << std::endl;

        // Define the callback for this specific adapter
        adapter.set_callback_on_scan_found([](SimpleBLE::Peripheral peripheral) 
        {
            // Print the device name (if any) and its MAC address
            std::string name = peripheral.identifier();
            if (name.empty()) 
            {
                name = "Unknown Device";
            }
            
            std::cout << "  -> [BLE] Found: " << name 
                      << " (" << peripheral.address() << ")" << std::endl;
        });

        // Start the blocking scan for 5000 milliseconds on this adapter
        std::cout << "[INFO] Scanning on " << adapter.identifier() << " for 5 seconds..." << std::endl;
        adapter.scan_for(5000);
    }

    std::cout << "\n[INFO] All scans complete." << std::endl;
}