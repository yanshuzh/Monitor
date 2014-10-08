#ifndef MYSQLMANAGER_H_    
#define MYSQLMANAGER_H_     

#include <mysql.h>    

#include <string>    
#include <iostream>       

#include <string.h>    

using namespace std;    

class MySQLManager    
{    
public:    
 
        MySQLManager(const char* hosts, const char* userName, const char* password, const char* dbName, unsigned int port);    
        ~MySQLManager();    
        void initConnection();     
        bool runSQLCommand(string sql);
	bool search_user(string sql);
	char* get_user_port(string sql);
	bool show_result(string sql);
        void destroyConnection();    
        bool getConnectionStatus();         
private:    
        bool IsConnected;       
        MYSQL mySQLClient;    
        unsigned int DEFAULTPORT;    
        const char* HOSTS;    
        const char* USERNAME;    
        const char* PASSWORD;    
        const char* DBNAME;   

};    

#endif /* MYSQLMANAGER_H_ */
