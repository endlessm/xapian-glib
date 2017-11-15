project('xapian-glib', 'c', 'cpp',
        version: '1.6.0',
        license: 'GPLv2.0+',
        meson_version: '>= 0.42.0',
        default_options: [
          'warning_level=1',
          'buildtype=debugoptimized',
          'c_std=c99',
          'cpp_std=c++11',
        ])

cpp = meson.get_compiler('cpp')

# Version
version = meson.project_version().split('.')
xapian_glib_major_version = version[0].to_int()
xapian_glib_minor_version = version[1].to_int()
xapian_glib_micro_version = version[2].to_int()

xapian_glib_api_version = '@0@.0'.format(xapian_glib_major_version)
xapian_glib_interface_age = xapian_glib_minor_version.is_even() ? xapian_glib_micro_version : 0
xapian_glib_binary_age = 100 * xapian_glib_minor_version + xapian_glib_micro_version

soversion = 0
libversion = '@0@.@1@.@2@'.format(soversion, xapian_glib_binary_age - xapian_glib_interface_age, xapian_glib_interface_age)

# Paths
xapian_glib_prefix = get_option('prefix')
xapian_glib_includedir = join_paths(get_option('prefix'), get_option('includedir'))
xapian_glib_libdir = join_paths(get_option('prefix'), get_option('libdir'))
xapian_glib_datadir = join_paths(get_option('prefix'), get_option('datadir'))
xapian_glib_api_name = '@0@-@1@'.format(meson.project_name(), xapian_glib_api_version)

root_inc = include_directories('.')

# Configuration
xapian_glib_conf = configuration_data()

# Compiler flags
common_cflags = []
common_ldflags = []
xapian_glib_debug_cflags = []

if cpp.get_id() == 'gcc' or cpp.get_id() == 'clang'
  test_cflags = [
    '-fno-strict-aliasing',
    '-Wpointer-arith',
    '-Wmissing-declarations',
    '-Wunused',
    '-Wcast-align',
    '-Wmissing-noreturn',
    '-Wmissing-format-attribute',
    '-Wmissing-include-dirs',
    '-Wlogical-op',
    '-Wno-uninitialized',
    '-Wno-shadow',
    '-Werror=format=2',
    '-Werror=redundant-decls',
    '-Werror=implicit',
    '-Werror=nonnull',
    '-Werror=init-self',
    '-Werror=main',
    '-Werror=missing-braces',
    '-Werror=sequence-point',
    '-Werror=return-type',
    '-Werror=trigraphs',
    '-Werror=array-bounds',
    '-Werror=write-strings',
    '-Werror=address',
    '-Werror=int-to-pointer-cast',
    '-Werror=pointer-to-int-cast',
    '-Werror=empty-body',
    '-Werror=write-strings',
    '-Werror=undef',
  ]

  # Request byte-alignment on ARM
  if host_machine.cpu_family() == 'arm'
    test_cflags += '-mstructure-size-boundary=8'
  endif
else
  test_cflags = []
endif

# Symbol visibility
visibility_cflags = []
if get_option('default_library') != 'static'
  if host_machine.system() == 'windows'
    xapian_glib_conf.set('DLL_EXPORT', true)
    xapian_glib_conf.set('_XAPIAN_GLIB_EXTERN', '__declspec(dllexport) extern')
    if cc.get_id() != 'msvc'
      visibility_cflags += '-fvisibility=hidden'
    endif
  else
    xapian_glib_conf.set('_XAPIAN_GLIB_EXTERN', '__attribute__((visibility("default"))) extern')
    visibility_cflags += [ '-fvisibility=hidden', '-fvisibility-inlines-hidden' ]
  endif
endif

# Check all compiler flags
foreach cflag: test_cflags
  if cpp.has_argument(cflag)
    common_cflags += cflag
  endif
endforeach

# Linker flags
if host_machine.system() == 'linux'
  foreach ldflag: [ '-Wl,-Bsymbolic', '-Wl,-z,relro', '-Wl,-z,now', ]
    if cpp.has_argument(ldflag)
      common_ldflags += ldflag
    endif
  endforeach
elif host_machine.system() == 'windows'
  common_ldflags += '-no-undefined'
elif host_machine.system() == 'darwin'
  common_ldflags += [ '-compatibility_version 1', '-current_version 1.0' ]
endif

# TODO: We may want a configuration argument to add `G_DISABLE_CHECKS`
# and `G_DISABLE_ASSERT` from the build, for specific build environments.
# On the other hand, those who need these symbols can inject them in their
# build as well.
buildtype = get_option('buildtype')
if buildtype.startswith('debug')
  xapian_glib_debug_cflags += [ '-DXAPIAN_GLIB_ENABLE_DEBUG', ]
elif buildtype == 'release'
  xapian_glib_debug_cflags += [ '-DG_DISABLE_CAST_CHECKS', ]
endif

# Dependencies
glib_req_version = '>= 2.40.0'
xapian_req_version = '>= 1.3.3'

gobject_dep = dependency('gobject-2.0', version: glib_req_version)
gio_dep = dependency('gio-2.0')
xapian_dep = dependency('xapian-core', version: xapian_req_version, required: false)
xapian_pc_dep = 'xapian-core'
if not xapian_dep.found()
  # Xapian 1.3 has a versioned xapian-core pkg-config file
  xapian_dep = dependency('xapian-core-1.3', version: xapian_req_version)
  xapian_pc_dep = 'xapian-core-1.3'
endif

# Modules
gnome = import('gnome')

subdir('xapian-glib')
subdir('examples')

if get_option('build_docs')
  subdir('doc')
endif
