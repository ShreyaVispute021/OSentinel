#include <iostream>
#include <iomanip>

#include "../include/system_monitor.h"

int main() {

    SystemMonitor monitor;

    CPUInfo cpu = monitor.getCPUInfo();
    MemoryInfo memory = monitor.getMemoryInfo();

    std::cout << "========================================\n";
    std::cout << "              OSentinel\n";
    std::cout << "     Linux System Resource Monitor\n";
    std::cout << "========================================\n\n";

    std::cout << "CPU Information\n";
    std::cout << "----------------------------------------\n";

    std::cout << "CPU Usage        : "
              << std::fixed
              << std::setprecision(2)
              << cpu.usagePercentage
              << "%\n\n";

    std::cout << "Memory Information\n";
    std::cout << "----------------------------------------\n";

    std::cout << "Total Memory     : "
              << memory.totalMemoryKB / 1024
              << " MB\n";

    std::cout << "Available Memory : "
              << memory.availableMemoryKB / 1024
              << " MB\n";

    std::cout << "Used Memory      : "
              << memory.usedMemoryKB / 1024
              << " MB\n";

    std::cout << "Memory Usage     : "
              << std::fixed
              << std::setprecision(2)
              << memory.usagePercentage
              << "%\n";

    return 0;
}