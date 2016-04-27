#include "src/base_mysql.h"
#include "common/base/string/number.h"
#include "thirdparty/gtest/gtest.h"
#include <iostream>

using namespace turtle;
using namespace toft;
using namespace std;

#define HOST "localhost"
#define PORT 0
#define USER "root"
#define PASSWD "root"
#define DB ""

TEST(BaseMysql,Test)
{
    BaseMysql base_mysql;

    ASSERT_EQ(0,base_mysql.Init());
    ASSERT_EQ(0,base_mysql.Connect(HOST,PORT,USER,PASSWD,DB));

    string create_db_sql = "create database if not exists test";
    ASSERT_EQ(0,base_mysql.Query(create_db_sql));
    ASSERT_EQ(0,base_mysql.Select_db("test"));
    
    string create_table_sql = "create table if not exists base_mysql_test"
        "(id int,name varchar(20),qq bigint)";
    ASSERT_EQ(0,base_mysql.Query(create_table_sql));

    // test insert
    int base_qq = 930030895;
    for ( int i = 0 ; i < 10 ; i++)
    {
        string id = NumberToString(i);
        string name = "turtle" + id;
        string qq = NumberToString(base_qq + i);
        string insert_sql = "insert into base_mysql_test(id,name,qq)values("
            + id + ",\"" + name + "\"," + qq + ")";
        EXPECT_EQ(0,base_mysql.Query(insert_sql));
    }

    // test update
    string update_sql = "update base_mysql_test set name = 'turtle' where id = 0";
    EXPECT_EQ(0,base_mysql.Query(update_sql));
    
    // test select
    string select_sql = "select * from base_mysql_test";
    EXPECT_EQ(0,base_mysql.Query(select_sql));
    MysqlRow row;
    while ( base_mysql.Fetch_row(&row) == 0)
    {
        int j = 0;
        string info = "[" + NumberToString(j) + "]";
        for ( int i = 0,len = row.Field_num();
                i < len;i++)
        {
            info += "[" + row.Field_name(i) + ":"
                + row[i] + "]";
        }
        cout << info << endl;
        j++;
    }

    // test delete
    for ( int i = 0 ; i < 10 ; i++)
    {
        string id = NumberToString(i);
        string delete_sql = "delete from base_mysql_test where id = " + id;
        EXPECT_EQ(0,base_mysql.Query(delete_sql));
    }

    // test close
    EXPECT_EQ(0,base_mysql.Close());
}

