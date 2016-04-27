#include "base/mysql/mysql.h"
#include <string>
#include <stdint.h>

using namespace std;

namespace turtle
{

class MysqlRow
{
    public:
        MysqlRow();
        string operator[] (int i);
        string Field_name(int i);
        enum_field_types Field_type(int i);
        uint32_t Field_num();
        uint32_t Row_num();
        MYSQL_RES* res;
        MYSQL_ROW row;
        MYSQL_FIELD* fields;
};

class BaseMysql
{
    public:
        BaseMysql();
        int Init();
        int Connect(string host,uint32_t port,string user,string passwd,
                string db = "",uint32_t client_flag = 0);
        int Select_db(string db);
        int Query(string sql);
        int Fetch_row(MysqlRow* mysql_row);
        int Close();
        string GetErrorMsg();
    
    private:
        MYSQL* mysql;
        MYSQL_RES* res;
};

}

