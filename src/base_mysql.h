#include <mysql.h>
#include <string>
#include <stdint.h>

using namespace std;

namespace turtle
{

class BaseMysql
{
    public:
        int Init();
        int Connect(string host,uint32_t port,string user,string passwd,string db = "");
        int Select_db(string db);
        int Query(string sql);
        int Close();
    private:
        MYSQL* mysql;
};

class MysqlRow
{
    public:
        string operator[] (int i);
};

}

