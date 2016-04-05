#include <mysql.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
    MYSQL mysql;
    unsigned int ret = 0;
    if ( mysql_init(&mysql) == NULL)
    {
        ret = mysql_errno(&mysql);
        printf("init mysql failed.[ret=%u][err=%s]",ret,mysql_error(&mysql));
        return ret;
    }

    if (mysql_real_connect(&mysql,"localhost","root","roo","mysql",0,NULL,0) == NULL)
    {
        ret = mysql_errno(&mysql);
        printf("connect mysql failed.[ret=%u][err=%s]",ret,mysql_error(&mysql));
        return ret;
    }

    printf("connect mysql successfully.\n");

    return 0;
}

