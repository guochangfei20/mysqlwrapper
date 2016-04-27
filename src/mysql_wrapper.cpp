/*
#***********************************************
#
#      Filename: mysql_wrapper.cpp
#
#        Author: turtle - 930030895@qq.com
#   Description: ---
#        Create: 2016-04-15 14:50:27
# Last Modified: 2016-04-27 13:49:05
#***********************************************/


#include "mysql_wrapper.h"
#include "common/base/string/number.h"

using namespace google::protobuf;
using namespace std;
using namespace toft;

namespace turtle
{
    int GenerateSQL(const Message* message,string separator,string* sql)
    {
        if ( message == NULL )
        {
            *sql = "";
            return 0;
        }

        const Descriptor* descriptor = message->GetDescriptor();
        const Reflection* reflection = message->GetReflection();
        int field_count = descriptor->field_count();

        for ( int i = 0 ; i < field_count ; i++ )
        {
            const FieldDescriptor* field = descriptor->field(i);
            if ( field->is_repeated() )
            {
                return UNSUPPORT_LABEL;
            }

            if ( reflection->HasField(*message,field) )
            {
                string name = field->name(),value;
                switch ( field->cpp_type() )
                {
                    case FieldDescriptor::CPPTYPE_STRING:
                        value = "\"" + reflection->GetString(*message,field) + "\"";
                        break;

                    #define CASE(cpp_type,type) case FieldDescriptor::CPPTYPE_##cpp_type:\
                        value = NumberToString(reflection->Get##type(*message,field));break

                    CASE(UINT64,UInt64);
                    CASE(INT64,Int64);
                    CASE(UINT32,UInt32);
                    CASE(INT32,Int32);
                    CASE(DOUBLE,Double);
                    CASE(FLOAT,Float);

                    default:
                        return UNSUPPORT_TYPE;
                }

                if ( !sql->empty() )
                {
                    *sql += " " + separator + " ";
                }
                
                *sql += name + " = " + value;
            }
        }
        
        return 0;
    }

    int MysqlWrapper::Create_table(string table,const Message* message)
    {
        if ( message == NULL )
        {
            return PARAM_ERROR;
        }

        const Descriptor* descriptor = message->GetDescriptor();
        int field_count = descriptor->field_count();
        
        string sql = "create table if not exists " + table + " (";
        string field_name,field_type;
        for ( int i = 0 ; i < field_count ; i ++ )
        {
            const FieldDescriptor* field_descriptor = descriptor->field(i);
            if ( field_descriptor->is_repeated() )
            {
                return UNSUPPORT_LABEL;
            }

            field_name = field_descriptor->name();
            switch ( field_descriptor->cpp_type() )
            {
                case FieldDescriptor::CPPTYPE_DOUBLE:
                case FieldDescriptor::CPPTYPE_FLOAT:
                    field_type = "double";
                    break;
                case FieldDescriptor::CPPTYPE_INT32:
                case FieldDescriptor::CPPTYPE_UINT32:
                case FieldDescriptor::CPPTYPE_BOOL:
                    field_type = "integer";
                    break;
                case FieldDescriptor::CPPTYPE_INT64:
                case FieldDescriptor::CPPTYPE_UINT64:
                    field_type = "bigint";
                    break;
                case FieldDescriptor::CPPTYPE_STRING:
                    field_type = "varchar(50)";
                    break;
                default:
                    return UNSUPPORT_TYPE;
            }

            sql += field_name + " " + field_type;
            if ( i != field_count - 1 )
            {
                sql += ",";
            }
        }

        sql += ")";

        int ret = 0;
        ret = Query(sql);
        
        return ret;
    }
    
    int MysqlWrapper::Insert(string table,const Message* message)
    {
        if ( message == NULL )
        {
            return PARAM_ERROR;
        }

        const Reflection* reflection = message->GetReflection();
        const Descriptor* descriptor = message->GetDescriptor();
        int field_count = descriptor->field_count();
        
        string fields,values;
        for ( int i = 0 ; i < field_count ; i++ )
        {
            const FieldDescriptor* field = descriptor->field(i);
            if ( field->is_repeated() )
            {
                return UNSUPPORT_LABEL;
            }

            fields += field->name();

            switch ( field->cpp_type() )
            {
                case FieldDescriptor::CPPTYPE_STRING:
                    values += "\"" + reflection->GetString(*message,field) + "\"";
                    break;

                #define CASE(type,cpptype) case FieldDescriptor::CPPTYPE_##type:\
                    values += NumberToString(reflection->Get##cpptype(*message,field));break
                
                CASE(INT64,Int64);
                CASE(INT32,Int32);
                CASE(UINT64,UInt64);
                CASE(UINT32,UInt32);
                CASE(DOUBLE,Double);
                CASE(FLOAT,Float);

                #undef CASE

                default:
                    return UNSUPPORT_TYPE;
            }
            
            if ( i != field_count - 1 )
            {
                fields += ",";
                values += ",";
            }
        }
        
        string sql = "insert into " + table
            + "(" + fields + ")values("
            + values + ")";

        int ret = Query(sql);

        return ret;
    }

    int MysqlWrapper::Update(string table,const Message* condition,const Message* message)
    {
        if ( message == NULL )
        {
            return PARAM_ERROR;
        }

        int ret = 0;
        string update,where;
        ret = GenerateSQL(message,",",&update);
        if ( ret != 0 )
        {
            return ret;
        }

        ret = GenerateSQL(condition,"and",&where);
        if ( ret != 0 )
        {
            return ret;
        }
        
        string sql = "update " + table + " set " + update;
        if ( !where.empty() )
        {
            sql += " where " + where;
        }

        ret = Query(sql);

        return ret;
    }

    int MysqlWrapper::InsertOrUpdate(string table,const Message* message)
    {
        if ( message == NULL )
        {
            return PARAM_ERROR;
        }

        const Descriptor* descriptor = message->GetDescriptor();
        const Reflection* reflection = message->GetReflection();
        int field_count = descriptor->field_count();
        string sql,fields,values,update;

        for ( int i = 0 ; i < field_count ; i++ )
        {
            const FieldDescriptor* field = descriptor->field(i);
            if ( field->is_repeated() )
            {
                return UNSUPPORT_LABEL;
            }
            
            string name = field->name(),value;
            
            switch ( field->cpp_type() )
            {
                case FieldDescriptor::CPPTYPE_STRING:
                    value = "\"" + reflection->GetString(*message,field) + "\"";
                    break;

                #define CASE(cpptype,type) case FieldDescriptor::CPPTYPE_##cpptype:\
                    value = NumberToString(reflection->Get##type(*message,field));break
                
                CASE(UINT64,UInt64);
                CASE(INT64,Int64);
                CASE(UINT32,UInt32);
                CASE(INT32,Int32);
                CASE(DOUBLE,Double);
                CASE(FLOAT,Float);

                #undef CASE
                    
                default:
                    return UNSUPPORT_TYPE;
            }

            fields += name;
            values += value;
            update += name + "=" + value;

            if ( i != field_count - 1 )
            {
                fields += ",";
                values += ",";
                update += ",";
            }
        }

        sql = "insert into " + table + "(" + fields + ")values("
            + values + ")on duplicate key update " + update;
        
        int ret = 0;
        ret = Query(sql);

        return ret;
    }

    int MysqlWrapper::Delete(string table,const Message* message)
    {
        int ret = 0;
        string where;

        ret = GenerateSQL(message,"and",&where);

        if ( ret != 0 )
        {
            return ret;
        }
        
        string sql = "delete from " + table + " where " + where;
        ret = Query(sql);

        return ret;
    }
} // end namespace turtle

