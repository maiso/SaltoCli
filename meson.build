project('SaltoCLI', 'cpp')

boost_dep = dependency('boost')
json_proj = subproject('nlohmann_json')
json_proj_dep = json_proj.get_variable('nlohmann_json_dep')

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

subdir('src')

subdir('test')
