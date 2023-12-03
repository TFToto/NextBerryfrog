#ifndef MYSQL_HPP
#define MYSQL_HPP

void mysql_connect (void);
void mysql_disconnect (void);
void mysql_write (int transmitter_id,int temp_dht_hic,int temp_dht_hif,int humidity_dht,int pressure_bmp,int altitude_bmp,int temp_bmp,int vcc_atmega,int vis_is,int uvindex_is);

#endif