/*c++ file
#***********************************************
#
#      Filename: performance_test.cpp
#
#        Author: turtle - 930030895@qq.com
#   Description: ---
#        Create: 2016-04-27 15:59:23
# Last Modified: 2016-05-16 14:50:50
#***********************************************/

#include "mysql_wrapper.h"
#include "src/student.pb.h"
#include "base/common/module.h"
#include "common/system/time/clock.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"
#include <stdio.h>
#include <string>
#include "common/base/string/number.h"

using namespace toft;
using namespace turtle;
using namespace std;

DEFINE_string(host,"localhost","");
DEFINE_int32(port,0,"");
DEFINE_string(user,"root","");
DEFINE_string(passwd,"root","");
DEFINE_string(db,"test","");
DEFINE_int32(test_num,1000,"");

void PrintResult(string test_case,int64_t total_time,int64_t max_time,int64_t avg_time,int32_t total_cnt,
        int32_t error_cnt,int32_t success_cnt)
{
    printf("%s:[total_time:%.2f][max_time:%.2f][avg_time:%.2f]\n",
            test_case.c_str(),total_time / 1000.0,max_time / 1000.0,
            avg_time / 1000.0);
}

int main(int argc, char* argv[])
{
    InitAllModules(&argc,&argv,false);
    
    MysqlWrapper mysql_wrapper;
    int ret = 0;
    ret = mysql_wrapper.Init();
    if ( ret != 0 )
    {
        LOG(ERROR) << "mysql init failed.[ret=" << ret << "].";
        return ret;
    }

    ret = mysql_wrapper.Connect(FLAGS_host,FLAGS_port,FLAGS_user,FLAGS_passwd,FLAGS_db);
    if ( ret != 0 )
    {
        LOG(ERROR) << "mysql connect failed.[ret=" << ret
            << "][msg=" << mysql_wrapper.GetErrorMsg() << "].";
        return ret;
    }
    
    uint64_t student_id = 201230672184;
    Student student;
    student.set_name("turtle");
    student.set_student_id(201230672185);
    student.set_gender(1);
    student.set_phone(13660543398);
    student.set_uin(930030895);
  

    int64_t total_time = 0,avg_time = 0,max_time = 0;
    int32_t total_cnt = FLAGS_test_num,error_cnt = 0,success_cnt = 0;
    int64_t start_time = 0,end_time = 0,cost_time = 0;

    //test insert
    for (int i = 0;i < FLAGS_test_num;i++)
    {
        student.set_student_id(student_id + i);
        start_time = RealtimeClock.MicroSeconds();
        ret = mysql_wrapper.Insert("student_wrapper",&student);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;

        if ( cost_time > max_time )
        {
            max_time = cost_time;
        }

        total_time += cost_time;

        if ( ret != 0)
        {
            error_cnt += 1;
            LOG(ERROR) << "wrapper insert failed.[ret=" << ret << "][msg="
                << mysql_wrapper.GetErrorMsg() << "].";
        }
        else
        {
            success_cnt += 1;
        }
    }
    
    avg_time = total_time / total_cnt;

    printf("Test insert:\n");
    PrintResult("Wrapper insert API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);

    total_time = 0,max_time = 0,error_cnt = 0,success_cnt = 0;
    for (int i = 0;i < FLAGS_test_num;i++)
    {
        string sql_insert = "insert into student_base(name,student_id,gender,phone,uin)"
            "values(\"turtle1\"," + NumberToString(student_id + i) + ",1,13660543398,930030895)";
        start_time = RealtimeClock.MicroSeconds();
        ret = mysql_wrapper.Query(sql_insert);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;

        if ( cost_time > max_time )
        {
            max_time = cost_time;
        }

        total_time += cost_time;

        if ( ret != 0)
        {
            error_cnt += 1;
            LOG(ERROR) << "base insert failed.[ret=" << ret << "][msg="
                << mysql_wrapper.GetErrorMsg() << "].";
        }
        else
        {
            success_cnt += 1;
        }
    }
    
    avg_time = total_time / total_cnt;

    PrintResult("Raw insert API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);
    
    // test update
    Student cond;
    cond.set_name("turtle");

    Student student2;
    student2.set_name("turtle2");

    string sql_update = "update student_base set name = \"turtle3\" where name = \"turtle1\"";

    total_time = 0,max_time = 0,error_cnt = 0,success_cnt = 0;
    for (int i = 0;i < FLAGS_test_num;i++)
    {
        start_time = RealtimeClock.MicroSeconds();
        ret = mysql_wrapper.Update("student_wrapper",&cond,&student2);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;
        
        if ( cost_time > max_time )
        {
            max_time = cost_time;
        }

        total_time += cost_time;

        if ( ret != 0)
        {
            error_cnt += 1;
            LOG(ERROR) << "wrapper update failed.[ret=" << ret << "][msg="
                << mysql_wrapper.GetErrorMsg() << "].";
        }
        else
        {
            success_cnt += 1;
        }
    }
    
    avg_time = total_time / total_cnt;

    printf("\n\nTest update:\n");
    PrintResult("Wrapper update API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);
    
    total_time = 0,max_time = 0,error_cnt = 0,success_cnt = 0;
    for (int i = 0;i < FLAGS_test_num;i++)
    {
        start_time = RealtimeClock.MicroSeconds();
        ret = mysql_wrapper.Query(sql_update);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;

        if ( cost_time > max_time )
        {
            max_time = cost_time;
        }

        total_time += cost_time;

        if ( ret != 0)
        {
            error_cnt += 1;
        }
        else
        {
            success_cnt += 1;
        }
    }
    
    avg_time = total_time / total_cnt;

    PrintResult("Raw insert API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);
    
    // test select
    Student cond2;
    cond2.set_name("turtle2");
    vector<Student> results;

    string sql_select = "select * from student_base where name = \"turtle3\"";
    
    total_time = 0,max_time = 0,error_cnt = 0,success_cnt = 0;
    for (int i = 0;i < FLAGS_test_num;i++)
    {
        results.clear();
        start_time = RealtimeClock.MicroSeconds();
        ret = mysql_wrapper.Select("student_wrapper",&cond2,&results);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;

        if ( cost_time > max_time )
        {
            max_time = cost_time;
        }

        total_time += cost_time;

        if ( ret != 0)
        {
            error_cnt += 1;
            LOG(ERROR) << "wrapper select failed.[ret=" << ret << "][msg="
                << mysql_wrapper.GetErrorMsg() << "].";
        }
        else
        {
            success_cnt += 1;
        }
    }
    
    avg_time = total_time / total_cnt;

    printf("\n\nTest select API:\n");
    PrintResult("Wrapper select API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);

    total_time = 0,max_time = 0,error_cnt = 0,success_cnt = 0;
    for (int i = 0;i < FLAGS_test_num;i++)
    {
        start_time = RealtimeClock.MicroSeconds();
        ret = mysql_wrapper.Query(sql_select);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;

        if ( cost_time > max_time )
        {
            max_time = cost_time;
        }

        total_time += cost_time;

        if ( ret != 0)
        {
            error_cnt += 1;
            LOG(ERROR) << "base select failed.[ret=" << ret << "][msg="
                << mysql_wrapper.GetErrorMsg() << "].";
        }
        else
        {
            success_cnt += 1;
        }
    }
    
    avg_time = total_time / total_cnt;

    PrintResult("Raw select API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);
    
    Student del;
    total_time = 0,max_time = 0;
    error_cnt = 0,success_cnt = 0;

    printf("\n\nTest delete api:\n");

    for (int i = 0;i < FLAGS_test_num;i++)
    {
        del.set_student_id(student_id + i);
        start_time = RealtimeClock.MicroSeconds();
        ret= mysql_wrapper.Delete("student_wrapper",&del);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;

        if (cost_time > max_time)
        {
            max_time = cost_time;
        }

        total_time += cost_time;
        
        if (ret != 0)
        {
            error_cnt++;
            LOG(ERROR) << "delete failed.[ret=" << ret << "].";
        }
        else
        {
            success_cnt++;
        }
    }

    avg_time = total_time / total_cnt;
    PrintResult("Wrapper delete API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);

    total_time = 0,max_time = 0;
    error_cnt = 0,success_cnt = 0;

    for (int i = 0;i < FLAGS_test_num;i++)
    {
        string delete_sql = "delete from student_base where student_id = " + NumberToString(student_id + i);
        start_time = RealtimeClock.MicroSeconds();
        ret = mysql_wrapper.Query(delete_sql);
        end_time = RealtimeClock.MicroSeconds();
        cost_time = end_time - start_time;

        if (cost_time > max_time)
        {
            max_time = cost_time;
        }

        total_time += cost_time;

        if (ret != 0)
        {
            LOG(ERROR) << "delete student_base failed.[ret=" << ret << "].";
            error_cnt++;
        }
        else
        {
            success_cnt++;
        }
    }

    avg_time = total_time / total_cnt;
    PrintResult("Base delete API:",total_time,max_time,avg_time,total_cnt,error_cnt,success_cnt);

    ret = mysql_wrapper.Close();
    if (ret != 0)
    {
        LOG(ERROR) << "mysql close failed.[ret=" << ret << "].";
    }

    return ret;
}

