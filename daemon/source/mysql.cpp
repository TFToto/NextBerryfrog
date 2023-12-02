/*
  CONNECT TO DATABASE ****************
*/
void mysql_connect (void) {

     //initialize MYSQL object for connections
     mysql1 = mysql_init(NULL);

     if(mysql1 == NULL) {
         fprintf(stderr, "ABB : %s\n", mysql_error(mysql1));
         return;
     }

    libconfig::Config config;
	if (config.lookupValue("dbhost", dbhost))
	if (config.lookupValue("dbname", dbname))
	if (config.lookupValue("dbuser", dbuser))
    if (config.lookupValue("dbpass", dbpass))

     //Connect to the database
     if(mysql_real_connect(mysql1, dbhost, dbuser, dbpass, dbname, 0, NULL, 0) == NULL) {
      fprintf(stderr, "%s\n", mysql_error(mysql1));
     } else {
         printf("Database connection to %s successful.\r\n",dbhost);
     }
}

/*
  DISCONNECT FROM DATABASE ****************
*/
void mysql_disconnect (void) {
    mysql_close(mysql1);
    printf( "Disconnected from database.\r\n");
}