#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

struct MemoryInfo {
    long totalMemoryKB;
    long availableMemoryKB;
    long usedMemoryKB;
    double usagePercentage;
};

struct CPUInfo {
    double usagePercentage;
};

class SystemMonitor {
public:
    MemoryInfo getMemoryInfo();
    CPUInfo getCPUInfo();
};

#endif