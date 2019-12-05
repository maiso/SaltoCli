project('SaltoCLI', 'cpp')
boost_dep = dependency('boost')
json_proj = subproject('nlohmann_json')
json_proj_dep = json_proj.get_variable('nlohmann_json_dep')

subdir('src')
subdir('test')

