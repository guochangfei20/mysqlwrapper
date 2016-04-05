bin = ../bin

$(bin)/base_mysql.o : base_mysql.cpp base_mysql.h
	g++ -c `mysql_config --cflags` base_mysql.cpp -o $(bin)/base_mysql.o

$(bin)/test : $(bin)/test_connect.o
	g++ -o $(bin)/test $(bin)/test_connect.o `mysql_config --libs`

$(bin)/test_connect.o : test_connect.cpp
	g++ -c `mysql_config --cflags` test_connect.cpp -o $(bin)/test_connect.o

.PHONY : clean test
clean :
	-cd $(bin);rm *.o test;cd -

test : $(bin)/test
	$(bin)/test

