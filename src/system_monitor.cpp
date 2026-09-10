#include "../include/system_monitor.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <utility>

MemoryInfo SystemMonitor::getMemoryInfo() {
    std::ifstream file("/proc/meminfo");

    std::string key;
    long value;
    std::string unit;

    long totalMemory = 0;
    long availableMemory = 0;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") {
            totalMemory = value;
        }

        if (key == "MemAvailable:") {
            availableMemory = value;
        }

        if (totalMemory > 0 && availableMemory > 0) {
            break;
        }
    }

    long usedMemory = totalMemory - availableMemory;

    double usagePercentage = 0.0;

    if (totalMemory > 0) {
        usagePercentage =
            (static_cast<double>(usedMemory) / totalMemory) * 100.0;
    }

    return {
        totalMemory,
        availableMemory,
        usedMemory,
        usagePercentage
    };
}

CPUInfo SystemMonitor::getCPUInfo() {
    auto readCPUStat = []() {
        std::ifstream file("/proc/stat");

        std::string cpu;

        long user, nice, system, idle;
        long iowait, irq, softirq, steal;

        file >> cpu
             >> user
             >> nice
             >> system
             >> idle
             >> iowait
             >> irq
             >> softirq
             >> steal;

        long idleTime = idle + iowait;

        long nonIdleTime =
            user + nice + system + irq + softirq + steal;

        long totalTime = idleTime + nonIdleTime;

        return std::pair<long, long>(totalTime, idleTime);
    };

    auto first = readCPUStat();

    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000)
    );

    auto second = readCPUStat();

    long totalDifference =
        second.first - first.first;

    long idleDifference =
        second.second - first.second;

    double usagePercentage = 0.0;

    if (totalDifference > 0) {
        usagePercentage =
            (static_cast<double>(
                totalDifference - idleDifference
            ) / totalDifference) * 100.0;
    }

    return {usagePercentage};
}