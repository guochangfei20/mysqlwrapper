/*c++ file
#***********************************************
#
#      Filename: mysql_wrapper_test.cpp
#
#        Author: turtle - 930030895@qq.com
#   Description: ---
#        Create: 2016-04-26 11:15:57
# Last Modified: 2016-04-27 17:08:17
#***********************************************/

#include "mysql_wrapper.h"
#include "thirdparty/gtest/gtest.h"
#include "src/student.pb.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace turtle;

#define HOST "localhost"
#define PORT 0
#define USER "root"
#define PASSWD "root"
#define DB "test"

TEST(MysqlWrapper,Test)
{
    MysqlWrapper mysql_wrapper;

    ASSERT_EQ(0,mysql_wrapper.Init());
    ASSERT_EQ(0,mysql_wrapper.Connect(HOST,PORT,USER,PASSWD,DB));
    
    Student student1;
    student1.set_name("turtle");
    student1.set_student_id(201230672184);
    student1.set_gender(1);
    student1.set_phone(13660543593);
    student1.set_uin(930030895);

    ASSERT_EQ(0,mysql_wrapper.Create_table("students",&student1));
    EXPECT_EQ(0,mysql_wrapper.Insert("students",&student1));

    Student condition1(student1);
    student1.set_name("turtle1");
    EXPECT_EQ(0,mysql_wrapper.Update("students",&condition1,&student1));
    
    vector<Student*> students;
    Student condition;
    condition.set_name("turtle1");
    EXPECT_EQ(0,mysql_wrapper.Select("students",&condition,&students));
    for (size_t i = 0;i < students.size();i++)
    {
        cout << "[" << i << "]" << students[i]->DebugString();
    }
    
    Teacher teacher1,teacher2,teacher3;
    teacher1.set_name("turtle2");
    teacher1.set_id(930030895);
    teacher1.set_gender(1);
    teacher1.set_age(23);

    teacher2.set_name("turtle3");
    teacher2.set_id(930030895);
    teacher2.set_gender(2);
    teacher2.set_age(24);

    teacher3.set_name("turtle4");
    teacher3.set_id(930030896);
    teacher3.set_gender(3);
    teacher3.set_age(25);
    
    EXPECT_EQ(0,mysql_wrapper.InsertOrUpdate("test_update_or_insert",&teacher1));
    EXPECT_EQ(0,mysql_wrapper.InsertOrUpdate("test_update_or_insert",&teacher2));
    EXPECT_EQ(0,mysql_wrapper.InsertOrUpdate("test_update_or_insert",&teacher3));

    EXPECT_EQ(0,mysql_wrapper.Delete("students",&student1));

    EXPECT_EQ(0,mysql_wrapper.Close());
}

