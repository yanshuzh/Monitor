#include "MySQLManager.h"    

MySQLManager::MySQLManager(const char* hosts, const char* userName, const char* password, const char* dbName, unsigned int port)    
{    
        IsConnected = false;  
	this->HOSTS=hosts;
	this->USERNAME=userName;
	this->PASSWORD=password;
	this->DBNAME=dbName;
	this->DEFAULTPORT=port;  
   
}    

MySQLManager::~MySQLManager()    
{    
        this ->destroyConnection();    
}    

void MySQLManager::initConnection()    
{    
        if ( IsConnected )    
        {//    已经连接到服务器    
                cout << "Is connected to server!" <<endl;    
                return;    
        } 
        mysql_init(&mySQLClient);//    初始化相关对象    
        if ( !mysql_real_connect( &mySQLClient, HOSTS, USERNAME, PASSWORD, DBNAME, DEFAULTPORT, NULL, 0) )    
        {//    连接到服务器
                cout << "**********************"<<endl;
                cout << "Error connection to Mysql: \n" << mysql_error(&mySQLClient) << endl;    
        }    
        IsConnected = true;//    修改连接标识
 	cout << "connected to mysql server success!" <<endl;      
}    
char* MySQLManager::get_user_port(string sql)    
{    	
        MYSQL_RES *res;    
        MYSQL_ROW row;    
    	MYSQL_FIELD *fd;
    	char column[32][32]; 
	bool temp;
        temp=runSQLCommand(sql);
	if(!temp)
	{
		return false;
	} 
	//cout<<"into"<<endl;  
        res = mysql_store_result(&mySQLClient);    
	//cout<<res<<endl;
        if(res)
         {
		row=mysql_fetch_row(res);
                return row[0];
         }  
        mysql_free_result(res);         //free result after you get the result    
        return 0;    
}
bool MySQLManager::show_result(string sql)    
{    	
        MYSQL_RES *res;    
        MYSQL_ROW row;    
    	MYSQL_FIELD *fd;
    	char column[32][32]; 
	bool temp;
        temp=runSQLCommand(sql);
	if(!temp)
	{
		return false;
	} 
	cout<<"into"<<endl;  
        res = mysql_store_result(&mySQLClient);    
	cout<<res<<endl;
            if(res)
            {
                int i,j;
                cout<<"number of result: "<<(unsigned long)mysql_num_rows(res)<<endl;
                for(i=0;fd=mysql_fetch_field(res);i++)//获取列名
                {
                    strcpy(column[i],fd->name);
                }
                j=mysql_num_fields(res);
                for(i=0;i<j;i++)
                { 
                    cout<<column[i]<<"  ";
                }
		cout<<endl;
                while(row=mysql_fetch_row(res))//获取具体的数据
                {
                    for(i=0;i<j;i++)
                    {
                        cout<<row[i]<<"      ";
                    }
                    cout<<endl;
                }
            }  
        mysql_free_result(res);         //free result after you get the result    
        return true;    
}
bool MySQLManager::search_user(string sql)
{  
	MYSQL_RES *res; 	
	bool temp;
        temp=runSQLCommand(sql);
	if(!temp)
	{
		return false;
	}
	res = mysql_store_result(&mySQLClient);
	if(!(unsigned long)mysql_num_rows(res)){
		return false;	
	}
	return true;
}

bool MySQLManager::runSQLCommand(string sql)  
{
	if ( !IsConnected )    
        {//    没有连接到服务器    
                cout << "Not connect to database!" << endl;    
                return false;    
        }    
        if ( sql.empty() )    
        {//    SQL语句为空    
                cout << "SQL is null!" << endl;    
                return false;    
        } 
	int i;
	i = mysql_real_query(&mySQLClient,sql.c_str(),(unsigned int)strlen(sql.c_str()));//    执行查询 
        if ( i < 0 )    
        {    
                cout << "Error query from database: \n" << mysql_error(&mySQLClient) << endl;    
                return false;    
        } 
	return true;
}

void MySQLManager::destroyConnection()    
{    
        mysql_close(&mySQLClient);    
        this ->IsConnected = false;    
}    

bool MySQLManager::getConnectionStatus()    
{    
        return IsConnected;    
}
