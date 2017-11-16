# Xapian-GLib

### GObject/C bindings for the Xapian API

Xapian-GLib is a wrapper library that offers a C API and GObject types for
the [Xapian](http://xapian.org) database.

Xapian-GLib allows accessing the Xapian API in various languages, through
the GObject Introspection API.

## Requirements

Xapian-GLib, unsurprisingly, requires:

 * Xapian ≥ 1.3.4
 * GObject ≥ 2.40

You will also need [Meson](http://mesonbuild.com) in order to build
Xapian-GLib.

## Building

In order to build Xapian-GLib you will need to clone the Git repository:

```sh
$ git clone git://github.com/endlessm/xapian-glib.git
$ cd xapian-glib
```

Then run:

```sh
$ meson _build .
$ ninja -C _build
$ meson test -C _build		# Run the test suite
```

To build the library.

You can install to the default location of `/usr/local` by using:

```sh
$ sudo ninja -C _build install
```

If you want to use an alternative location for the installation, use the
`--prefix` configuration switch with Meson. See `meson --help` for
additional configuration options.

## Documentation

The API reference for the latest version of Xapian-GLib is available
[online](http://endlessm.github.io/xapian-glib/docs/).

## License

Like Xapian, Xapian-GLib is released under the terms of the GNU General
Public License, version 2.0 or (at your option) later.

See the file COPYING for details.
