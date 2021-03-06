ent_conf = configuration_data()
ent_conf.set('PACKAGE', 'Xapian-GLib')
ent_conf.set('PACKAGE_BUGREPORT', 'https://github.com/endlessm/xapian-glib/issues')
ent_conf.set('PACKAGE_NAME', meson.project_name())
ent_conf.set('PACKAGE_STRING', meson.project_name())
ent_conf.set('PACKAGE_TARNAME', meson.project_name() + '-' + meson.project_version())
ent_conf.set('PACKAGE_URL', 'http://endlessm.github.io/xapian-glib')
ent_conf.set('PACKAGE_VERSION', meson.project_version())
ent_conf.set('PACKAGE_API_VERSION', xapian_glib_api_version)
configure_file(input: 'gtkdocentities.ent.in', output: 'gtkdocentities.ent', configuration: ent_conf)

ignored_headers = [
  'config.h',

  'xapian-database-private.h',
  'xapian-document-private.h',
  'xapian-error-private.h',
  'xapian-mset-private.h',
  'xapian-posting-source-private.h',
  'xapian-query-private.h',
  'xapian-stem-private.h',
  'xapian-stopper-private.h',
  'xapian-term-iterator-private.h',
  'xapian-value-posting-source-private.h',

  'xapian-glib.h',
]

glib_prefix = dependency('glib-2.0').get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')

docpath = join_paths(xapian_glib_datadir, 'gtk-doc', 'html')

gnome.gtkdoc(
  'xapian-glib2',
  main_xml: 'xapian-glib2-docs.xml',
  src_dir: [
    'xapian-glib',
  ],
  dependencies: xapian_glib_dep,
  gobject_typesfile: 'xapian-glib2.types',
  scan_args: [
    '--rebuild-types',
    '--ignore-decorators=_XAPIAN_GLIB_EXTERN',
    '--ignore-headers=' + ' '.join(ignored_headers),
  ],
  mkdb_args: [
    '--source-suffixes=c,h,cc',
  ],
  fixxref_args: [
    '--html-dir=@0@'.format(docpath),
    '--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
    '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
    '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gio')),
  ],
  install: true,
)
