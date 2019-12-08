SaltoCli_incdir = include_directories('.', '.', is_system : true)

SaltoCli_libsources = files('SaltoCli.cpp', 'Console.cpp')

SaltoCli_lib = static_library('SaltoCliLib',
  sources: SaltoCli_libsources,
  dependencies: [boost_dep, json_proj_dep],
  cpp_args : '-std=c++17',
)

SaltoCli_dep = declare_dependency(
  include_directories : SaltoCli_incdir,
  link_with: SaltoCli_lib,
  link_args : ['-lpthread', '-lboost_system'],
)

exe = executable('SaltoCLI', ['main.cpp'], 
                 dependencies : [json_proj_dep, SaltoCli_dep],
                 )