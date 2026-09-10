#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../include/system_monitor.h"
#include "../include/process_monitor.h"

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

    ProcessMonitor processMonitor;

    auto processes =
        processMonitor.getProcesses();

    std::sort(
        processes.begin(),
        processes.end(),
        [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.cpuUsage > b.cpuUsage;
        }
    );

    std::cout << "\nTop CPU Processes\n";
    std::cout << "--------------------------------------------------------------\n";

    std::cout
        << std::left
        << std::setw(8)  << "PID"
        << std::setw(20) << "NAME"
        << std::setw(8)  << "PPID"
        << std::setw(15) << "STATE"
        << std::setw(12) << "MEM(MB)"
        << std::setw(10) << "THREADS"
        << std::setw(10) << "CPU(%)"
        << "\n";

    std::cout << "--------------------------------------------------------------\n";

    int count = 0;

    for (const auto& process : processes) {

        std::cout
            << std::left
            << std::setw(8)  << process.pid
            << std::setw(20) << process.name
            << std::setw(8)  << process.ppid
            << std::setw(15) << process.state
            << std::setw(12) << process.memoryKB / 1024
            << std::setw(10) << process.threads
            << std::setw(10)
            << std::fixed
            << std::setprecision(2)
            << process.cpuUsage
            << "\n";

        count++;

        if (count == 15) {
            break;
        }
    }

    return 0;
}