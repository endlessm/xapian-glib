tests = [
  'database',
  'document',
  'query-parser',
  'sortable-serialise',
  'stem',
]

foreach t: tests
  test_bin = executable(t, t + '.c',
                        include_directories: include_directories('.'),
                        dependencies: xapian_glib_dep,
                        c_args: common_cflags + xapian_glib_debug_cflags)
  test(t, test_bin,
       env: [
         'G_TEST_SRCDIR=@0@'.format(meson.current_source_dir()),
         'G_TEST_BUILDDIR=@0@'.format(meson.current_build_dir()),
       ],
       args: [
         '--tap', '-k',
       ])
endforeach
