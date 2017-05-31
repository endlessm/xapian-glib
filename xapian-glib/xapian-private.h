#ifndef __XAPIAN_PRIVATE_H__
#define __XAPIAN_PRIVATE_H__

#include <xapian.h>

#define _XAPIAN_VERSION_ENCODE_INTERNAL(maj,min,rev) \
  ((maj * 10000) + (min * 100) + rev)

#define _XAPIAN_CUR_VERSION \
  _XAPIAN_VERSION_ENCODE_INTERNAL(XAPIAN_MAJOR_VERSION,XAPIAN_MINOR_VERSION,XAPIAN_REVISION)

/* This is an internal version check macro for the Xapian version */
#define XAPIAN_CHECK_VERSION_INTERNAL(maj,min,rev) \
   (_XAPIAN_CUR_VERSION >= _XAPIAN_VERSION_ENCODE_INTERNAL(maj,min,rev))

#endif
