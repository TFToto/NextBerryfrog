#include "../include/mysql.hpp"
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>

#include "log.hpp"

MYSQL *mysql1;

// config_t cfg, *cf;
// const config_setting_t *retries;

/*
  CONNECT TO DATABASE ****************
*/
void mysqlConnect(const char *dbhost, const char *dbname, const char *dbuser, const char *dbpass) {

    //initialize MYSQL object for connections
    mysql1 = mysql_init(NULL);

    if(mysql1 == NULL) {
        LOGGER_ERROR("MySQL error: %s", mysql_error(mysql1));
        return;
    }

    //Connect to the database
    if(mysql_real_connect(mysql1, dbhost, dbuser, dbpass, dbname, 0, NULL, 0) == NULL) {
        LOGGER_ERROR("MySQL error: %s", mysql_error(mysql1));
    } else {
        LOGGER_INFO("Database connection to %s successful.", dbhost);
    }
}

/*
  DISCONNECT FROM DATABASE ****************
*/
void mysqlDisconnect (void) {

    mysql_close(mysql1);
    LOGGER_INFO("Disconnected from database.");
}

void mysqlWrite (int transmitter_id,int temp_dht_hic,int temp_dht_hif,int humidity_dht,int pressure_bmp,int altitude_bmp,int temp_bmp,int vcc_atmega,int vis_is,int uvindex_is) {
   
	float db_temp_dht_hic = (float) temp_dht_hic / 100;
	float db_temp_dht_hif = (float) temp_dht_hif / 100;
	float db_humidity_dht = (float) humidity_dht / 100;
	float db_pressure_bmp = (float) pressure_bmp / 10;
	float db_altitude_bmp = (float) altitude_bmp / 10;
	float db_temp_bmp     = (float) temp_bmp / 100;
	float db_vcc_atmega   = (float) vcc_atmega / 1000;
	float db_uvindex_is   = (float) uvindex_is / 1000;

	printf("Write datas into db for transmitter ID: %i\n",transmitter_id);
	printf("Temp DHT hiC: %.2f\n",db_temp_dht_hic);
	printf("Temp DHT hiF: %.2f\n",db_temp_dht_hif);
	printf("Humidity of DHT: %.2f\n",db_humidity_dht);
	printf("Pressure of BMP: %.1f\n",db_pressure_bmp);
	printf("Altitude of BMP: %.1f\n",db_altitude_bmp);
	printf("Temp of BMP: %.2f\n",db_temp_bmp);
	printf("VCC in ATMega: %.3f\n",db_vcc_atmega);
	printf("Vis of IS: %i\n",vis_is);
	printf("UV index: %.3f\n",db_uvindex_is);

    if(mysql1 != NULL) {
		
		//Retrieve all data from alarm_times
		char buf[512];
		sprintf(
			buf,
			"INSERT INTO measurements (transmitter_id,vcc_atmega,temp_dht_hic,temp_dht_hif,humidity_dht,pressure_bmp,altitude_bmp,temp_bmp,vis_is,uvindex_is) VALUES (%i,%.3f,%.2f,%.2f,%.2f,%.1f,%.1f,%.2f,%i,%.3f)",
					transmitter_id,
					db_vcc_atmega,
					db_temp_dht_hic,
					db_temp_dht_hif,
					db_humidity_dht,
					db_pressure_bmp,
					db_altitude_bmp,
					db_temp_bmp,
					vis_is,
					db_uvindex_is
		);

        if (mysql_query(mysql1, buf)) { 
             fprintf(stderr, "%s\n", mysql_error(mysql1));
             return;
        }
    }
}