#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    int ppid;
    std::string name;
    std::string state;
    long memoryKB;
    int threads;
    double cpuUsage;
};

class ProcessMonitor {
public:
    std::vector<ProcessInfo> getProcesses();
};

#endif