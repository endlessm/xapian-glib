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
 * SECTION:xapian-posting-source
 * @Title: XapianPostingSource
 * @short_description: Posting source
 *
 * #XapianPostingSource is an abstract class that serves as a base
 * for posting sources implementation.
 *
 * See #XapianValuePostingSource or #XapianValueWeightPostingSource
 * for examples of that.
 */

#include "config.h"

#include "xapian-posting-source-private.h"
#include "xapian-error-private.h"

#define XAPIAN_POSTING_SOURCE_GET_PRIVATE(obj) \
  ((XapianPostingSourcePrivate *) xapian_posting_source_get_instance_private ((XapianPostingSource *) (obj)))

typedef struct _XapianPostingSourcePrivate   XapianPostingSourcePrivate;

struct _XapianPostingSourcePrivate
{
  Xapian::PostingSource *mPostingSource;
};

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (XapianPostingSource, xapian_posting_source,
                                  G_TYPE_OBJECT,
                                  G_ADD_PRIVATE (XapianPostingSource))

  /* Dummy posting source class with unimplemented functions.
   * Used in xapian_posting_source_constructed. */
class GenericPostingSource : public Xapian::PostingSource {
  public:
    GenericPostingSource (XapianPostingSource *aWrapper)
      : mWrapper(aWrapper) {}

    virtual Xapian::doccount get_termfreq_min() const {
        return -1;
    }

    virtual Xapian::doccount get_termfreq_est() const {
        return -1;
    }

    virtual Xapian::doccount get_termfreq_max() const {
        return -1;
    }

    virtual Xapian::docid get_docid() const {
        return -1;
    }

    virtual void next(double min_wt) {
    }

    virtual bool at_end() const {
        return false;
    }

    virtual void init(const Xapian::Database& db) {
    }

  private:
    XapianPostingSource *mWrapper;
};

/*< private >
 * xapian_posting_source_get_internal:
 * @self: a #XapianPostingSource
 *
 * Retrieves the `Xapian::PostingSource` object used by @self.
 *
 * Returns: (transfer none): a pointer to the internal posting source instance
 *
 * Since: 2.0
 */
Xapian::PostingSource*
xapian_posting_source_get_internal (XapianPostingSource *self) 
{
  XapianPostingSourcePrivate *priv = XAPIAN_POSTING_SOURCE_GET_PRIVATE (self);

  return priv->mPostingSource;
}

/*< private >
 * xapian_posting_source_set_internal:
 * @self: a #XapianPostingSource
 * @aPostingSource: a `Xapian::PostingSource` instance
 *
 * Sets the internal posting source instance wrapped by @self, clearing
 * any existing instance if needed.
 */
void
xapian_posting_source_set_internal (XapianPostingSource   *self,
                                    Xapian::PostingSource *aPostingSource)
{
  XapianPostingSourcePrivate *priv = XAPIAN_POSTING_SOURCE_GET_PRIVATE (self);

  delete priv->mPostingSource;

  priv->mPostingSource = aPostingSource;
}

/**
 * xapian_posting_source_get_description:
 * @self: a #XapianPostingSource
 *
 * Retrieves a string describing the #XapianPostingSource.
 *
 * Typically, this function is used when debugging.
 *
 * Returns: (transfer full): a description of the posting source
 * Since: 2.0
 */
char *
xapian_posting_source_get_description (XapianPostingSource *self)
{
  XapianPostingSourcePrivate *priv = XAPIAN_POSTING_SOURCE_GET_PRIVATE (self);

  std::string desc = priv->mPostingSource->get_description();

  return g_strdup (desc.c_str ());
}

static void
xapian_posting_source_finalize (GObject *object)
{
  XapianPostingSource *posting_source = XAPIAN_POSTING_SOURCE (object);
  XapianPostingSourcePrivate *priv = XAPIAN_POSTING_SOURCE_GET_PRIVATE (posting_source);

  delete priv->mPostingSource;

  G_OBJECT_CLASS (xapian_posting_source_parent_class)->finalize (object);
}

static void
xapian_posting_source_constructed (GObject *gobject)
{
  XapianPostingSource *posting_source = XAPIAN_POSTING_SOURCE (gobject);
  XapianPostingSourcePrivate *priv = XAPIAN_POSTING_SOURCE_GET_PRIVATE (posting_source);

  G_OBJECT_CLASS (xapian_posting_source_parent_class)->constructed (gobject);

  if (priv->mPostingSource == NULL)
    priv->mPostingSource = new GenericPostingSource (posting_source);
}

static void
xapian_posting_source_class_init (XapianPostingSourceClass *klass)
{
  GObjectClass * gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->constructed = xapian_posting_source_constructed;
  gobject_class->finalize = xapian_posting_source_finalize;
}

static void
xapian_posting_source_init (XapianPostingSource *posting_source)
{
}
