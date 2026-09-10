#include "../include/process_monitor.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <cctype>

namespace fs = std::filesystem;

bool isNumeric(const std::string& str) {
    if (str.empty()) return false;

    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    return true;
}

long getTotalCPUTime() {
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

    return user + nice + system + idle +
           iowait + irq + softirq + steal;
}

long getProcessCPUTime(int pid) {

    std::ifstream file(
        "/proc/" + std::to_string(pid) + "/stat"
    );

    if (!file.is_open()) {
        return -1;
    }

    std::string line;
    std::getline(file, line);

    /*
        /proc/[pid]/stat contains many fields.
        Fields 14 and 15 are:
        utime -> user CPU time
        stime -> kernel CPU time
    */

    size_t closeParen = line.rfind(')');

    if (closeParen == std::string::npos) {
        return -1;
    }

    std::string afterName =
        line.substr(closeParen + 2);

    std::istringstream iss(afterName);

    std::vector<std::string> fields;

    std::string field;

    while (iss >> field) {
        fields.push_back(field);
    }

    /*
        Since PID and process name were removed,
        utime becomes index 11
        stime becomes index 12
    */

    if (fields.size() <= 12) {
        return -1;
    }

    long utime = std::stol(fields[11]);
    long stime = std::stol(fields[12]);

    return utime + stime;
}

std::vector<ProcessInfo> readProcesses() {

    std::vector<ProcessInfo> processes;

    for (const auto& entry : fs::directory_iterator("/proc")) {

        std::string filename =
            entry.path().filename().string();

        if (!isNumeric(filename)) {
            continue;
        }

        int pid = std::stoi(filename);

        std::ifstream statusFile(
            "/proc/" + filename + "/status"
        );

        if (!statusFile.is_open()) {
            continue;
        }

        ProcessInfo process;

        process.pid = pid;
        process.ppid = 0;
        process.name = "Unknown";
        process.state = "Unknown";
        process.memoryKB = 0;
        process.threads = 0;
        process.cpuUsage = 0.0;

        std::string line;

        while (std::getline(statusFile, line)) {

            std::istringstream iss(line);

            std::string key;

            iss >> key;

            if (key == "Name:") {
                iss >> process.name;
            }

            else if (key == "State:") {
                std::string stateCode;
                iss >> stateCode;

                std::getline(iss, process.state);

                process.state =
                    stateCode + process.state;
            }

            else if (key == "PPid:") {
                iss >> process.ppid;
            }

            else if (key == "VmRSS:") {
                iss >> process.memoryKB;
            }

            else if (key == "Threads:") {
                iss >> process.threads;
            }
        }

        processes.push_back(process);
    }

    return processes;
}

std::vector<ProcessInfo> ProcessMonitor::getProcesses() {

    auto processes = readProcesses();

    std::unordered_map<int, long> firstTimes;

    for (const auto& process : processes) {

        long cpuTime =
            getProcessCPUTime(process.pid);

        if (cpuTime >= 0) {
            firstTimes[process.pid] = cpuTime;
        }
    }

    long totalCPU1 = getTotalCPUTime();

    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000)
    );

    long totalCPU2 = getTotalCPUTime();

    long totalDifference =
        totalCPU2 - totalCPU1;

    for (auto& process : processes) {

        long secondTime =
            getProcessCPUTime(process.pid);

        if (secondTime < 0) {
            continue;
        }

        if (firstTimes.find(process.pid)
            == firstTimes.end()) {
            continue;
        }

        long processDifference =
            secondTime - firstTimes[process.pid];

        if (totalDifference > 0) {

            process.cpuUsage =
                (static_cast<double>(
                    processDifference
                ) / totalDifference)
                * 100.0;
        }
    }

    return processes;
}