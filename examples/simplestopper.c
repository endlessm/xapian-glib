#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <xapian-glib.h>

typedef struct {
  XapianStopper parent_instance;

  GPtrArray *terms;
} SimpleStopper;

typedef struct {
  XapianStopperClass parent_class;
} SimpleStopperClass;

G_DEFINE_TYPE (SimpleStopper, simple_stopper, XAPIAN_TYPE_STOPPER)

static gboolean
simple_stopper_is_stop_term (XapianStopper *stopper,
                             const char    *term)
{
  SimpleStopper *self = (SimpleStopper *) stopper;
  int i;

  for (i = 0; i < self->terms->len; i++)
    {
      if (g_str_equal (term, self->terms->pdata[i]))
        return TRUE;
    }

  return FALSE;
}

static void
simple_stopper_finalize (GObject *gobject)
{
  SimpleStopper *self = (SimpleStopper *) gobject;

  g_ptr_array_unref (self->terms);

  G_OBJECT_CLASS (simple_stopper_parent_class)->finalize (gobject);
}

static void
simple_stopper_class_init (SimpleStopperClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  XapianStopperClass *stopper_class = XAPIAN_STOPPER_CLASS (klass);

  gobject_class->finalize = simple_stopper_finalize;

  stopper_class->is_stop_term = simple_stopper_is_stop_term;
}

static void
simple_stopper_init (SimpleStopper *self)
{
  self->terms = g_ptr_array_new_with_free_func (g_free);
}

static void
simple_stopper_add_term (SimpleStopper *self,
                         const char    *term)
{
  g_ptr_array_add (self->terms, g_strdup (term));
}

int
main (int   argc,
      char *argv[])
{
  SimpleStopper *stopper;
  int i;

  stopper = g_object_new (simple_stopper_get_type (), NULL);

  for (i = 1; i < argc; i++)
    {
      simple_stopper_add_term (stopper, argv[i]);

      g_assert (xapian_stopper_is_stop_term (XAPIAN_STOPPER (stopper), argv[i]));
    }

  g_object_unref (stopper);

  return 0;
}
