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
 * SECTION:xapian-stopper
 * @Title: XapianStopper
 * @short_description: stopper
 *
 * Abstract class for stop-word decision functor.
 */

#include "config.h"

#include "xapian-stopper.h"
#include "xapian-stopper-private.h"
#include "xapian-error-private.h"

#define XAPIAN_STOPPER_GET_PRIVATE(obj) \
  ((XapianStopperPrivate *) xapian_stopper_get_instance_private ((XapianStopper *) (obj)))

typedef struct _XapianStopperPrivate   XapianStopperPrivate;

struct _XapianStopperPrivate
{
  Xapian::Stopper *mStopper;
};

class GenericStopper : public Xapian::Stopper {
  public:
    GenericStopper (XapianStopper *aWrapper)
      : mWrapper(aWrapper) {}

    virtual bool operator() (const std::string &term) const {
        XapianStopperClass *klass = XAPIAN_STOPPER_GET_CLASS (mWrapper);

        return klass->is_stop_term (mWrapper, term.c_str ());
    }

    virtual std::string get_description() const {
        XapianStopperClass *klass = XAPIAN_STOPPER_GET_CLASS (mWrapper);

        char *str = klass->get_description (mWrapper);

        std::string res (str);

        g_free (str);

        return res;
    }

  private:
    XapianStopper *mWrapper;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (XapianStopper, xapian_stopper, G_TYPE_OBJECT)

static void
xapian_stopper_finalize (GObject *self)
{
  XapianStopperPrivate *priv = XAPIAN_STOPPER_GET_PRIVATE (self);

  delete priv->mStopper;

  G_OBJECT_CLASS (xapian_stopper_parent_class)->finalize (self);
}

/*< private >
 * xapian_stopper_get_internal:
 * @self: a #XapianStopper
 *
 * Retrieves the `Xapian::Stopper` object used by @self.
 *
 * Returns: (transfer none): a pointer to the internal stopper instance
 */
Xapian::Stopper *
xapian_stopper_get_internal (XapianStopper *self)
{
  XapianStopperPrivate *priv = XAPIAN_STOPPER_GET_PRIVATE (self);

  return priv->mStopper;
}

/*< private >
 * xapian_stopper_set_internal:
 * @self: a #XapianStopper
 * @aDB: a `Xapian::Stopper` instance
 *
 * Sets the internal stopper instance wrapped by @self, clearing
 * any existing instance if needed.
 */
void
xapian_stopper_set_internal (XapianStopper   *self,
                             Xapian::Stopper *aStopper)
{
  XapianStopperPrivate *priv = XAPIAN_STOPPER_GET_PRIVATE (self);

  delete priv->mStopper;

  priv->mStopper = aStopper;
}

/**
 * xapian_stopper_get_description:
 * @self: a #XapianStopper
 *
 * Returns a string describing this object.
 *
 * Returns: (transfer full): description of the stopper
 * Since 1.2
 */
char *
xapian_stopper_get_description (XapianStopper *self)
{
  Xapian::Stopper *stopper = xapian_stopper_get_internal (self);

  std::string desc = stopper->get_description ();

  return g_strdup (desc.c_str ());
}

/**
 * xapian_stopper_is_stop_term:
 * @self: a #XapianStopper
 * @term: a term
 *
 * Checks whether @term is a stopper term.
 *
 * Returns: %TRUE if @term is a stopper term
 *
 * Since: 1.2
 */
gboolean
xapian_stopper_is_stop_term (XapianStopper *self,
                             const char    *term)
{
  Xapian::Stopper *stopper = xapian_stopper_get_internal (self);

  return (*stopper) (std::string (term));
}

static void
xapian_stopper_constructed (GObject *gobject)
{
  XapianStopper *stopper = XAPIAN_STOPPER (gobject);
  XapianStopperPrivate *priv = XAPIAN_STOPPER_GET_PRIVATE (stopper);

  if (priv->mStopper == NULL)
    priv->mStopper = new GenericStopper (stopper);

  G_OBJECT_CLASS (xapian_stopper_parent_class)->constructed (gobject);
}

static gboolean
xapian_stopper_real_is_stop_term (XapianStopper *stopper,
                                  const char    *term)
{
  return FALSE;
}

static char *
xapian_stopper_real_get_description (XapianStopper *stopper)
{
  return g_strdup ("<Unimplemented XapianStopper::get_description()>");
}

static void
xapian_stopper_class_init (XapianStopperClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->constructed = xapian_stopper_constructed;
  gobject_class->finalize = xapian_stopper_finalize;

  klass->is_stop_term = xapian_stopper_real_is_stop_term;
  klass->get_description = xapian_stopper_real_get_description;
}

static void
xapian_stopper_init (XapianStopper *stopper)
{
}
