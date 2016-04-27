cc_library(
    name = 'base_mysql',
    srcs = [
        'base_mysql.cpp',
    ],
    deps = [
        '#pthread',
        '#z',
        '#m',
        '#dl',
        '#mysqlclient',
    ]
)

cc_library(
    name = 'mysql_wrapper',
    srcs = [
        'mysql_wrapper.cpp',
    ],
    deps = [
        ':base_mysql',
        ':student',
        '//thirdparty/protobuf:protobuf',
        '//common/base/string:_number',
    ]
)

cc_test(
    name = 'base_mysql_test',
    srcs = [
        'base_mysql_test.cpp',
    ],
    deps = [
        '//common/base/string:_number',
        ':base_mysql',
    ]
)

cc_test(
    name = 'mysql_wrapper_test',
    srcs = [
        'mysql_wrapper_test.cpp',
    ],
    deps = [
        ':mysql_wrapper',
    ]
)

proto_library(
    name = 'student',
    srcs = [
        'student.proto',
    ],
    deps = [
    
    ]
)

