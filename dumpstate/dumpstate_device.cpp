#include <android-base/file.h>
#include <android-base/properties.h>
#include "DumpstateUtil.h"

#include <sstream>
#include <string>
#include <vector>

using android::os::dumpstate::CommandOptions;
using android::os::dumpstate::DumpFileToFd;
using android::base::GetProperty;
using android::base::ReadFileToString;

// Function to split a string based on a delimiter
static std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

extern "C" void dumpstate_device_handler(const int fd, [[maybe_unused]] const bool full) {
    std::string tempStr;

    dprintf(fd, "====== Enter device handler ======\n");

    // Parse kernel parameters
    if (ReadFileToString("/proc/cmdline", &tempStr)) {
        std::vector<std::string> kernelParams = splitString(tempStr, ' ');
        for (const auto& param : kernelParams) {
            if (param.find("mdss_mdp.panel=") != std::string::npos) {
                dprintf(fd, "------ Display Panel info ------\n");
                std::vector<std::string> panelParams = splitString(param, ':');
                for (const auto& panelParam : panelParams) {
                    if (panelParam.find("qcom,mdss_dsi_") != std::string::npos) {
                        dprintf(fd, "Panel name: %s\n", panelParam.c_str());
                    } else if (panelParam.find("bklic=") != std::string::npos) {
                        tempStr = panelParam.substr(6);
                        if (tempStr == "01")
                            dprintf(fd, "Backlight IC: LM3697\n");
                        else if (tempStr == "24")
                            dprintf(fd, "Backlight IC: KTD3136\n");
                        else
                            dprintf(fd, "Backlight IC: Unknown (%s)\n", tempStr.c_str());
                    } else if (panelParam.find("wpoint=") != std::string::npos) {
                        dprintf(fd, "White point: %s\n", panelParam.substr(7).c_str());
                    }
                }
                dprintf(fd, "\n");
            }
        }
    } else {
        dprintf(fd, "*** Error reading /proc/cmdline ***\n");
    }

    // Fingerprint
    dprintf(fd, "------ Fingerprint variant ------\n");
    tempStr = GetProperty("vendor.fingerprint.hwmdl", "none");
    if (tempStr == "none")
        dprintf(fd, "None\n");
    else if (tempStr == "fingerprint")
        dprintf(fd, "FPC\n");
    else if (tempStr == "gf_fingerprint")
        dprintf(fd, "Goodix\n");
    else
        dprintf(fd, "Invalid\n");
    dprintf(fd, "\n");

    // FM Radio
    DumpFileToFd(fd, "FM Internal Antenna", "/sys/devices/virtual/fm/fm_lan_enable/lan_enable");

    // Huaqin
    DumpFileToFd(fd, "Huaqin CTP", "/sys/devices/virtual/huaqin/interface/hw_info/ctp");
    DumpFileToFd(fd, "Huaqin PCBA_CONFIG", "/sys/devices/virtual/huaqin/interface/hw_info/pcba_config");

    // Touchscreen
    DumpFileToFd(fd, "Touchscreen DT2W", "/proc/sys/dev/dt2w");

    return;
}
