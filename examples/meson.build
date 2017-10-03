examples = [
  'simplesearch',
  'simplestopper',
]

foreach e: examples
  executable(e, e + '.c',
             include_directories: include_directories('.'),
             dependencies: xapian_glib_dep,
             c_args: common_cflags)
endforeach
