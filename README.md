# Xapian-GLib

### GObject/C bindings for the Xapian API

Xapian-GLib is a wrapper library that offers a C API and GObject types for
the Xapian database.

Xapian-GLib allows accessing the Xapian API in various languages, through
the GObject Introspection API.

## Requirements

Xapian-GLib, unsurprisingly, requires:

 * Xapian
 * GObject

## Building

In order to build Xapian-GLib you will need to clone the Git repository:

    $ git clone git://github.com/endlessm/xapian-glib.git
    $ cd xapian-glib

Then run the `autogen.sh` script and `make`:

    $ ./autogen.sh
    $ make

You can install to the default location of `/usr/local` by using:

    # make install

If you want to use an alternative location, use the `--prefix` configuration
switch with the `autogen.sh` script.

## License

Like Xapian, Xapian-GLib is released under the terms of the GNU General
Public License, version 2.0 or (at your option) later.

See the file COPYING for details.
