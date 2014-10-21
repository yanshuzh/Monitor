#include "MySQLManager.h"
 
#include <iostream>
#include <vector>
 
using namespace std;

 
int main() 
{
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "0715", "", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus())
    {
	cout<<"begin"<<endl;
/*	if(mysql->Add_database("create database if not exists StudentData"))
	{
		cout << "success to add"<<endl;
        }
	else
		cout << "执行失败" << endl;
*/
	if(mysql->runSQLCommand("use StudentData"))
        {
            
                cout << "success"<<endl;
        }
        else
            cout << "执行失败" << endl;
	if(mysql->show_result("select * from userdata"))
        {
            
                cout << "success"<<endl;
        }
        else
            cout << "执行失败" << endl;
/*	if(mysql->runSQLCommand("create table if not exists userdata(id int auto_increment primary key,user_name varchar(20),user_passwd varchar(20))"))
	{           
                cout << "success"<<endl;
        }
        else
            cout << "执行失败" << endl;
*/
	if(mysql->runSQLCommand("insert into userdata (user_name,user_passwd) values ('1234','1234'),('7890','7890')"))
        {
            
                cout << "success"<<endl;
        }
        else
            cout << "执行失败" << endl;

/*        if(mysql->Show_databases("show tables"))
        {
            
                cout << "success"<<endl;
        }
        else
            cout << "执行失败" << endl;
*/
    }

    else
        cout << "连接未建立" << endl;
        
    return 0;
}
