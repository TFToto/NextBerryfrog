#include <chrono>
#include <iostream>
#include <libconfig.h++>
#include <thread>

#include "command-line-parser.hpp"
#include "daemon.hpp"
#include "log.hpp"
#include "mysql.hpp"
#include "../rc-switch/RCSwitch.h"

RCSwitch mySwitch;

static char *conf_file_name = NULL;
static char *pid_file_name = NULL;
static int pid_fd = -1;
static char *app_name = NULL;
static FILE *log_stream;

int loop = 10;
int count, n, enabled;

using namespace std;

// This function will be called when the daemon receive a SIGHUP signal.
void reload() {
    LOG_INFO("Reload function called.");
}

int main(int argc, char** argv) {

    // Command line and config file example
    CommandLineParser commandLine(argc, argv);
    if (commandLine.cmdOptionExist("--config")) {
        const std::string configFileName = commandLine.getCmdOptionValue("--config");
        LOG_INFO("Config file name = ", configFileName);
        libconfig::Config config;
        config.readFile(configFileName.c_str());
        std::string test{};
        config.lookupValue("test", test);
        LOG_INFO("Config option test = ", test);
    }

    // The Daemon class is a singleton to avoid be instantiate more than once
    Daemon& daemon = Daemon::instance();

    // Set the reload function to be called in case of receiving a SIGHUP signal
    daemon.setReloadFunction(reload);

    // Daemon main loop
    int count = 0;
    while (daemon.IsRunning()) {
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOG_INFO("The next-berryfrog process ended gracefully.");
}
