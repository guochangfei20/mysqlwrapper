/*
#***********************************************
#
#      Filename: mysql_wrapper.h
#
#        Author: turtle - 930030895@qq.com
#   Description: ---
#        Create: 2016-04-15 14:08:28
# Last Modified: 2016-04-27 15:32:56
#***********************************************/

#include "base_mysql.h"
#include "thirdparty/protobuf/message.h"
#include "thirdparty/protobuf/descriptor.h"
#include "common/base/string/number.h"
#include <string>
#include <vector>

#define PARAM_ERROR -1
#define UNSUPPORT_LABEL -2
#define UNSUPPORT_TYPE -3

using namespace google::protobuf;
using namespace std;
using namespace toft;

namespace turtle
{

int GenerateSQL(const Message* message,string separator,string* sql);

class MysqlWrapper : public BaseMysql
{
    public:
        int Create_table(string table,const Message* message);
        int Insert(string table,const Message* message);
        template<typename Msg>
        int Select(string table,const Msg* condition,vector<Msg*>* results);
        template<typename Msg>
        int Select(string table,string where,const Msg* message,vector<Msg*>* results);
        int Update(string table,const Message* condition,const Message* message);
        int InsertOrUpdate(string table,const Message* message);
        int Delete(string table,const Message* condition);
};

template<typename Msg>
int MysqlWrapper::Select(string table,const Msg* condition,vector<Msg*>* results)
{
    if ( condition == NULL)
    {
        return PARAM_ERROR;
    }

    int ret = 0;
    string where;

    ret = GenerateSQL(condition," and ",&where);
    if ( ret != 0 )
    {
        return ret;
    }

    ret = Select(table,where,condition,results);

    return ret;
}

template<typename Msg>
int MysqlWrapper::Select(string table,string where,const Msg* message,vector<Msg*>* results)
{
    if ( message == NULL)
    {
        return PARAM_ERROR;
    }

    int ret = 0;
    string sql = "select * from " + table + " where " + where;
    ret = Query(sql);
    if ( ret != 0)
    {
        return ret;
    }

    MysqlRow row;
    while ( Fetch_row(&row) == 0 )
    {
        Msg* item = message->New();
        const Descriptor* descriptor = item->GetDescriptor();
        const Reflection* reflection = item->GetReflection();
        for ( int i = 0,len = row.Field_num() ; i < len ; i++ )
        {
            string name = row.Field_name(i);
            string value = row[i];

            const FieldDescriptor* field = descriptor->FindFieldByName(name);
            if ( field->is_repeated() )
            {
                return UNSUPPORT_LABEL;
            }

            switch ( field->cpp_type() )
            {
                case FieldDescriptor::CPPTYPE_STRING:
                    reflection->SetString(item,field,value);
                    break;

                #define CASE(cpptype,type,type_cpp) case FieldDescriptor::CPPTYPE_##cpptype:\
                    {type_cpp num;\
                        StringToNumber(value,&num);\
                        reflection->Set##type(item,field,num);}break

                    CASE(UINT64,UInt64,uint64);
                    CASE(INT64,Int64,int64);
                    CASE(UINT32,UInt32,uint32);
                    CASE(INT32,Int32,int32);
                    CASE(DOUBLE,Double,double);
                    CASE(FLOAT,Float,float);

                #undef CASE

                default:
                    return UNSUPPORT_TYPE;
            }
        }

        results->push_back(item);
    }

    return ret;
}

} //  end namespace turtle

