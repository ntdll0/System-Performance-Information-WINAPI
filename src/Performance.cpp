#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <vector>

double GetSystemMemoryUsagePercentage() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    return static_cast<double>(memStatus.dwMemoryLoad);
}

unsigned long long GetSystemMemoryTotal() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    return memStatus.ullTotalPhys;
}

unsigned long long GetSystemMemoryUsed() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    return memStatus.ullTotalPhys - memStatus.ullAvailPhys;
}

double GetCurrentProcessMemoryUsagePercentage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    return static_cast<double>(physMemUsedByMe) / GetSystemMemoryTotal() * 100.0;
}

std::string GetSystemUptime() {
    DWORD ticks = GetTickCount();
    int hours = ticks / (1000 * 60 * 60);
    int minutes = (ticks % (1000 * 60 * 60)) / (1000 * 60);
    return std::to_string(hours) + ":" + std::to_string(minutes);
}

std::string GetPCModel() {
    HKEY hKey;
    std::string model;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD size = 0;
        RegQueryValueEx(hKey, L"SystemProductName", NULL, NULL, NULL, &size);
        std::vector<BYTE> buffer(size);
        RegQueryValueEx(hKey, L"SystemProductName", NULL, NULL, buffer.data(), &size);
        model.assign(buffer.begin(), buffer.end());
        RegCloseKey(hKey);
    }

    model.erase(std::remove(model.begin(), model.end(), '\0'), model.end());
    return model;
}

std::string GetHostname() {
    DWORD bufferSize = MAX_PATH;
    CHAR osName[MAX_PATH];
    if (GetComputerNameExA(ComputerNameDnsHostname, osName, &bufferSize))
    return std::string(osName);
    else return NULL;
}

int main() {
    std::cout << "System RAM Usage Percentage: " << GetSystemMemoryUsagePercentage() << "%" << std::endl;
    std::cout << "System RAM Usage: " << GetSystemMemoryUsed() / (1024 * 1024) << " MB / " << GetSystemMemoryTotal() / (1024 * 1024) << " MB" << std::endl;
    std::cout << "Current Process RAM Usage: " << GetCurrentProcessMemoryUsagePercentage() << "%" << std::endl;
    std::cout << "System Uptime: " << GetSystemUptime() << std::endl;
    std::cout << "PC Model: " << GetPCModel() << std::endl;
    std::cout << "HostName: " << GetHostname() << std::endl;

    return 0;
}
