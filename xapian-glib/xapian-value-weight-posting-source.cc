/* Copyright 2014  Endless Mobile
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * SECTION:xapian-value-weight-posting-source
 * @Title: XapianValueWeightPostingSource
 * @short_description: Value weight posting source
 *
 * #XapianValueWeightPostingSource is a posting source  which reads weights
 * from a value slot.
 *
 * This returns entries for all documents in the given database which have a 
 * non empty values in the specified slot. It returns a weight calculated
 * by applying sortable_unserialise to the value stored in the slot
 * (so the values stored should probably have been calculated by applying
 * sortable_serialise to a floating point number at index time).
 */

#include "config.h"
#include "xapian-posting-source-private.h"
#include "xapian-value-weight-posting-source.h"
#include "xapian-error-private.h"

static void initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianValueWeightPostingSource, xapian_value_weight_posting_source,
                         XAPIAN_TYPE_VALUE_POSTING_SOURCE,
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init))

static gboolean
xapian_value_weight_posting_source_init_internal (GInitable    *self,
                                                  GCancellable *cancellable,
                                                  GError      **error)
{
  XapianPostingSource *pSource = XAPIAN_POSTING_SOURCE (self);
  Xapian::ValueWeightPostingSource *mValueWeightPostingSource;

  try
    {
      guint slot = xapian_value_posting_source_get_slot (XAPIAN_VALUE_POSTING_SOURCE (self));
      mValueWeightPostingSource = new Xapian::ValueWeightPostingSource (slot);
      xapian_posting_source_set_internal (pSource, (Xapian::PostingSource *) mValueWeightPostingSource);

      return TRUE;
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return FALSE;
    }
}

static void
initable_iface_init (GInitableIface *iface)
{
  iface->init = xapian_value_weight_posting_source_init_internal;
}

static void
xapian_value_weight_posting_source_init (XapianValueWeightPostingSource *self)
{
}

static void
xapian_value_weight_posting_source_class_init (XapianValueWeightPostingSourceClass *klass)
{
}

/**
 * xapian_value_weight_posting_source_new:
 * @slot: slot in the database entries to use for source
 * @error: return location for a #GError, or %NULL
 *
 * If the initializion failed, @error is set, and this function
 * will return %NULL.
 *
 * Returns: (transfer full): the newly created #XapianValueWeightPostingSource instance
 *
 * Since: 2.0
 */
XapianValueWeightPostingSource *
xapian_value_weight_posting_source_new (unsigned int slot,
                                        GError **error)
{
  return static_cast<XapianValueWeightPostingSource *> (g_initable_new (XAPIAN_TYPE_VALUE_WEIGHT_POSTING_SOURCE,
                                                                        NULL, error,
                                                                        "slot", slot,
                                                                        NULL));
}
