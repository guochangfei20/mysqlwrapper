#include "base_mysql.h"

using namespace std;

namespace turtle
{
    int BaseMysql::Init()
    {
        mysql = mysql_init(NULL);
        if (mysql == NULL)
        {
            return -1;
        }
    
        return 0;
    }

    int BaseMysql::Connect(string host,uint32_t port,string user,string passwd,string db)
    {
        MYSQL* ret;
        if (db.empty())
        {
            ret = mysql_real_connect(mysql,host.c_str(),user.c_str(),
                passwd.c_str(),NULL,port,NULL,0);
        }
        else
        {
            ret = mysql_real_connect(mysql,host.c_str(),user.c_str(),
                passwd.c_str(),db.c_str(),port,NULL,0);
        }

        if (ret == NULL)
        {
            return -1;
        }

        return 0;
    }

    int BaseMysql::Select_db(string db)
    {
        int ret = 0;
        ret = mysql_select_db(mysql,db.c_str());

        return ret;
    }

    int BaseMysql::Query(string sql) 
    {
        int ret = 0;
        ret = mysql_query(mysql,sql.c_str());

        return ret;
    }

    int BaseMysql::Close()
    {
        mysql_close(mysql);

        return 0;
    }
}
