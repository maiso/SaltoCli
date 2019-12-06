e = executable('testprog', 'ConsoleTest.cpp', 
                dependencies : [gtest_dep, gmock_dep, json_proj_dep, SaltoCli_dep],
                cpp_args : '-std=c++17')
                
test('gtest test', e)