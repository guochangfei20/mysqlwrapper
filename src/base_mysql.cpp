#include "base_mysql.h"

using namespace std;

namespace turtle
{
    BaseMysql::BaseMysql()
    {
        mysql = NULL;
        res = NULL;
    }

    int BaseMysql::Init()
    {
        mysql = mysql_init(NULL);
        if (mysql == NULL)
        {
            return -1;
        }
    
        return 0;
    }

    int BaseMysql::Connect(string host,uint32_t port,string user,string passwd,string db,uint32_t client_flag)
    {
        MYSQL* ret;
        if (db.empty())
        {
            ret = mysql_real_connect(mysql,host.c_str(),user.c_str(),
                passwd.c_str(),NULL,port,NULL,client_flag);
        }
        else
        {
            ret = mysql_real_connect(mysql,host.c_str(),user.c_str(),
                passwd.c_str(),db.c_str(),port,NULL,client_flag);
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
        res = mysql_store_result(mysql);

        return ret;
    }
    
    int BaseMysql::Fetch_row(MysqlRow* mysql_row)
    {
        if (res == NULL)
        {
            return -1;
        }

        mysql_row->row = mysql_fetch_row(res);
        
        if (mysql_row->res != res)
        {
            mysql_row->res = res;
            mysql_row->fields = mysql_fetch_fields(res);
        }

        if (mysql_row->row == NULL)
        {
            return -2;
        }

        return 0;
    }

    int BaseMysql::Close()
    {
        mysql_close(mysql);

        if (res != NULL)
        {
            mysql_free_result(res);
        }

        return 0;
    }

    string BaseMysql::GetErrorMsg()
    {
        return string(mysql_error(mysql));
    }

    MysqlRow::MysqlRow()
    {
        res = NULL;
    }

    string MysqlRow::operator[] (int i)
    {
        return string(row[i]);    
    }

    string MysqlRow::Field_name(int i)
    {
        return string(fields[i].name);
    }

    enum_field_types MysqlRow::Field_type(int i)
    {
        return fields[i].type;
    }

    uint32_t MysqlRow::Field_num()
    {
        return mysql_num_fields(res);
    }

    uint32_t MysqlRow::Row_num()
    {
        return mysql_num_rows(res);
    }
}
