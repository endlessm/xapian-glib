xapian_version_conf = configuration_data()
xapian_version_conf.set('XAPIAN_GLIB_MAJOR_VERSION', xapian_glib_major_version)
xapian_version_conf.set('XAPIAN_GLIB_MINOR_VERSION', xapian_glib_minor_version)
xapian_version_conf.set('XAPIAN_GLIB_MICRO_VERSION', xapian_glib_micro_version)
xapian_version_conf.set('XAPIAN_GLIB_VERSION', meson.project_version())
configure_file(input: 'xapian-glib-version.h.in',
               output: 'xapian-glib-version.h',
               configuration: xapian_version_conf,
               install: true,
               install_dir: join_paths(xapian_glib_includedir, xapian_glib_api_name))

configure_file(output: 'config.h',
               configuration: xapian_glib_conf)

xapian_glib_headers = [
  'xapian-glib.h',

  'xapian-database.h',
  'xapian-document.h',
  'xapian-enquire.h',
  'xapian-enums.h',
  'xapian-glib-macros.h',
  'xapian-glib-types.h',
  'xapian-mset.h',
  'xapian-posting-source.h',
  'xapian-query-parser.h',
  'xapian-query.h',
  'xapian-simple-stopper.h',
  'xapian-stem.h',
  'xapian-stopper.h',
  'xapian-term-generator.h',
  'xapian-term-iterator.h',
  'xapian-utils.h',
  'xapian-value-posting-source.h',
  'xapian-value-weight-posting-source.h',
  'xapian-writable-database.h',
]

install_headers(xapian_glib_headers, subdir: xapian_glib_api_name)

xapian_glib_sources = [
  'xapian-database.cc',
  'xapian-document.cc',
  'xapian-enquire.cc',
  'xapian-enums.cc',
  'xapian-error.cc',
  'xapian-mset.cc',
  'xapian-mset-iterator.cc',
  'xapian-posting-source.cc',
  'xapian-query.cc',
  'xapian-query-parser.cc',
  'xapian-simple-stopper.cc',
  'xapian-stem.cc',
  'xapian-stopper.cc',
  'xapian-term-generator.cc',
  'xapian-term-iterator.cc',
  'xapian-utils.cc',
  'xapian-value-posting-source.cc',
  'xapian-value-weight-posting-source.cc',
  'xapian-writable-database.cc',
]

xapian_glib = shared_library(xapian_glib_api_name, xapian_glib_sources,
                             soversion: soversion,
                             version: libversion,
                             include_directories: root_inc,
                             dependencies: [ xapian_dep, gobject_dep, gio_dep ],
                             cpp_args: common_cflags + visibility_cflags + xapian_glib_debug_cflags + [
                               '-DG_LOG_DOMAIN="XapianGlib"',
                               '-DG_LOG_USE_STRUCTURED=1',
                               '-DXAPIAN_GLIB_COMPILATION=1',
                             ],
                             link_args: common_ldflags,
                             install: true)

xapian_glib_dep = declare_dependency(link_with: xapian_glib,
                                     include_directories: [
                                       root_inc,
                                       include_directories('.')
                                     ],
                                     dependencies: [ xapian_dep, gobject_dep, gio_dep ])

pkg = import('pkgconfig')
pkg.generate(libraries: xapian_glib,
             name: 'Xapian-GLib',
             description: 'Xapian bindings for GLib',
             version: meson.project_version(),
             filebase: xapian_glib_api_name,
             subdirs: xapian_glib_api_name,
             requires: [ 'gobject-2.0', 'gio-2.0', 'xapian-core' ])

if not meson.is_cross_build()
  gnome.generate_gir(xapian_glib,
                     sources: xapian_glib_headers + xapian_glib_sources,
                     namespace: 'Xapian',
                     nsversion: xapian_glib_api_version,
                     identifier_prefix: 'Xapian',
                     symbol_prefix: 'xapian',
                     export_packages: xapian_glib_api_name,
                     includes: [ 'GObject-2.0', 'Gio-2.0' ],
                     extra_args: [
                       '--quiet',
                       '--c-include="xapian-glib.h"',
                       '-DXAPIAN_GLIB_COMPILATION',
                     ],
                     install: true)
endif

subdir('tests')
