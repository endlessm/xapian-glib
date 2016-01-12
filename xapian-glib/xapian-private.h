#ifndef __XAPIAN_PRIVATE_H__
#define __XAPIAN_PRIVATE_H__

/* This is an internal version check macro for the Xapian version */
#define XAPIAN_CHECK_VERSION_INTERNAL(maj,min,rev) \
  ((maj) > XAPIAN_MAJOR || \
   ((maj) == XAPIAN_MAJOR && (min) > XAPIAN_MINOR) || \
   ((maj) == XAPIAN_MAJOR && (min) == XAPIAN_MINOR && (rev) >= XAPIAN_REVISION))

#endif
