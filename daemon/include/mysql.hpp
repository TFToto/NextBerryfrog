#ifndef MYSQL_HPP
#define MYSQL_HPP

#include <string>

void mysqlConnect(const char *dbhost, const char *dbname, const char *dbuser, const char *dbpass);
void mysqlDisconnect (void);
void mysqlWrite (int transmitter_id,int temp_dht_hic,int temp_dht_hif,int humidity_dht,int pressure_bmp,int altitude_bmp,int temp_bmp,int vcc_atmega,int vis_is,int uvindex_is);

#endif