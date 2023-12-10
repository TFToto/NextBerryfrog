#include <chrono>
#include <iostream>
#include <libconfig.h++>
#include <libconfig.h>
#include <thread>
#include <libconfig.h>
#include <mysql/mysql.h>
#include <getopt.h>
#include <syslog.h>

#include "command-line-parser.hpp"
#include "daemon.hpp"
#include "log.hpp"
#include "mysql.hpp"
#include "../rc-switch/RCSwitch.h"

// MYSQL *mysql1;
RCSwitch mySwitch;

static char *pid_file_name = NULL;
static int pid_fd = -1;
static char *app_name = NULL;
static FILE *log_stream;

int count, n, enabled;

using namespace std;

// This function will be called when the daemon receive a SIGHUP signal.
void reload() {
    LOGGER_INFO("Reload function called.");
}

int main(int argc, char** argv) {

    // static struct option long_options[] = {
	// 	{"conf_file", required_argument, 0, 'c'},
	// 	{"test_conf", required_argument, 0, 't'},
	// 	{"log_file", required_argument, 0, 'l'},
	// 	{"help", no_argument, 0, 'h'},
	// 	{"daemon", no_argument, 0, 'd'},
	// 	{"pid_file", required_argument, 0, 'p'},
	// 	{NULL, 0, 0, 0}
	// };

    //Init some variables
	// int value, option_index = 0, ret;
	// char *log_file_name = NULL;
	// int start_daemonized = 0;
	//Default PIN for 433MHz receiver data
	// int PIN = 2;

	// app_name = argv[0];

    /* Open system log and write message to it */
	// openlog(argv[0], LOG_PID|LOG_CONS, LOG_DAEMON);
	// syslog(LOG_INFO, "Started %s", app_name);

    // 	/* Try to open log file to this daemon */
	// if(log_file_name != NULL) {
	// 	log_stream = fopen(log_file_name, "a+");
	// 	if (log_stream == NULL)
	// 	{
	// 		syslog(LOG_ERR, "Can not open log file: %s, error: %s",
	// 			log_file_name, strerror(errno));
	// 		log_stream = stdout;
	// 	}
	// } else {
	// 	log_stream = stdout;
	// }

    /* Read configuration from config file */
	// read_conf_file(0);

	// if (config_lookup_int(cf, "loop", &loop));
	// if (config_lookup_int(cf, "rx_PIN", &rx_PIN));

    //Init the wiringPi interface
	// if(wiringPiSetup() == -1) {
    //    printf("wiringPiSetup failed, exiting...");
    //    return 0;
    // }

    // int pulseLength = 0;
    // if (argv[1] != NULL) pulseLength = atoi(argv[1]);

    // mySwitch = RCSwitch();
	// if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
    // mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2

    // Command line and config file example
    CommandLineParser commandLine(argc, argv);
    if (!commandLine.cmdOptionExist("--config")) {

        LOGGER_ERROR("Missing configuration file input");
        exit(0);
    } 
    
    const std::string configFileName = commandLine.getCmdOptionValue("--config");
    LOGGER_INFO("Config file name = ", configFileName);
    libconfig::Config config;
    config.readFile(configFileName.c_str());

    // The Daemon class is a singleton to avoid be instantiate more than once
    Daemon& daemon = Daemon::instance();

    // Set the reload function to be called in case of receiving a SIGHUP signal
    daemon.setReloadFunction(reload);

    //Define output value for database
	int receiver_id;
	int sensor_id;
	int measured_value;
	int temp_dht_hic = 0;
	int check11 = 0;
	int temp_dht_hif = 0;
	int check12 = 0;
	int humidity_dht = 0;
	int check13 = 0;
	int pressure_bmp = 0;
	int check14 = 0;
	int altitude_bmp = 0;
	int check15 = 0;
	int temp_bmp = 0;
	int check16 = 0;
	int vcc_atmega = 0;
	int check17 = 0;
	int vis_is = 0;
	int check18 = 0;
	int uvindex_is = 0;
	int check19 = 0;
	int check98 = 0;
	int check99 = 0;
	int check10 = 0;
     
    if(wiringPiSetup() == -1) {
        LOGGER_ERROR("wiringPiSetup failed, exiting");
        exit(0);
    }

    int pulseLength = 0;
    if (argv[1] != NULL) pulseLength = atoi(argv[1]);

    mySwitch = RCSwitch();
    if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);

    int rx_PIN{};
    config.lookupValue("rx_PIN", rx_PIN);
    LOGGER_INFO("rx_PIN is ", rx_PIN);
     mySwitch.enableReceive(rx_PIN);  // Receiver on interrupt 0 => that is pin #2

    LOGGER_INFO("Next-Berryfrog ready to receive");

    while (daemon.IsRunning()) {

        if (mySwitch.available()) {
    
            unsigned long value = mySwitch.getReceivedValue();
        
            if (value == 0) {
                LOGGER_WARNING("Unknown encoding");
            } else {

                // printf("Received %lu\n", mySwitch.getReceivedValue() );
                				int transmitter_id = value/10000000;
				int sensor_id = (value/100000)%100;
				int measured_value = value%10000;

				//START of sending loop
				if ((value/10)%100 == 98 && transmitter_id + (value/10)%100 != check98) {
					check98 = transmitter_id + (value/10)%100;
					check99 = 0;
					continue;
				}

				if (check10 == 0 && (value/10)%100 != 98
				  || check10 == 0 && (value/10)%100 != 99) {

					//Do not try to get transmitter ID in start or and of transmition loop
					check10 = transmitter_id;
				}

				if (sensor_id == 11 && check11 != transmitter_id + sensor_id) {
					temp_dht_hic = measured_value;
					check11 = transmitter_id + sensor_id;
					continue;
				}

				if (sensor_id == 12 && check12 != transmitter_id + sensor_id) {
					temp_dht_hif = measured_value;
					check12 = transmitter_id + sensor_id;
					continue;
				} 

				if (sensor_id == 13 && check13 != transmitter_id + sensor_id) {
					humidity_dht = measured_value;
					check13 = transmitter_id + sensor_id;
					continue;
				} 

				if (sensor_id == 14 && check14 != transmitter_id + sensor_id) {
					pressure_bmp = measured_value;
					check14 = transmitter_id + sensor_id;
					continue;
				} 

				if (sensor_id == 15 && check15 != transmitter_id + sensor_id) {
					altitude_bmp = measured_value;
					check15 = transmitter_id + sensor_id;
					continue;
				} 

				if (sensor_id == 16 && check16 != transmitter_id + sensor_id) {
					temp_bmp = measured_value;
					check16 = transmitter_id + sensor_id;
					continue;
				} 

				if (sensor_id == 17 && check17 != transmitter_id + sensor_id) {
					vcc_atmega = measured_value;
					check17 = transmitter_id + sensor_id;
					continue;
				}

				if (sensor_id == 18 && check18 != transmitter_id + sensor_id) {
					vis_is = measured_value;
					check18 = transmitter_id + sensor_id;
					continue;
				}

				if (sensor_id == 19 && check19 != transmitter_id + sensor_id) {
					uvindex_is = measured_value;
					check19 = transmitter_id + sensor_id;
					continue;
				}

                if ((value/10)%100 == 99 && transmitter_id + (value/10)%100 != check99) {

					//Writes collected datas in database
                    std::string dbhost{};
                    std::string dbname{};
                    std::string dbuser{};
                    std::string dbpass{};
                    config.lookupValue("dbhost", dbhost);
                    config.lookupValue("dbname", dbname);
                    config.lookupValue("dbuser", dbuser);
                    config.lookupValue("dbpass", dbpass);

                    const char * host = dbhost.c_str();
                    const char * name = dbname.c_str();
                    const char * user = dbuser.c_str();
                    const char * pass = dbpass.c_str();

                    mysqlConnect(host, name, user, pass);
                    mysqlWrite(check10,temp_dht_hic,temp_dht_hif,humidity_dht,pressure_bmp,altitude_bmp,temp_bmp,vcc_atmega,vis_is,uvindex_is);						
					mysqlDisconnect();

					//Reset checks for next loop
					check10 = 0;
					check11 = 0;
					check12 = 0;
					check13 = 0;
					check14 = 0;
					check15 = 0;
					check16 = 0;
					check17 = 0;
					check18 = 0;
					check19 = 0;
					check98 = 0;
					check99 = transmitter_id + (value/10)%100;
				} else {
					continue;
				}
            }
    
            fflush(stdout);
            mySwitch.resetAvailable();
        }
        usleep(100);
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOGGER_INFO("The next-berryfrog process ended gracefully.");
}
