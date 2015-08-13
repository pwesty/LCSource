#ifndef KSEEK_H
#define KSEEK_H

#include "KRFReadLib.h"

/**
 * An implementation of seek using the KRFReadLib interface.
 * This implements seek by using kread, kreset and kgetSize.
 *
 * WARNING: Using this naively can result in a large performance loss!
 * For instance, kseek(SEEK_END), ktell(), kseek(SEEK_SET) is much more slow
 * than kgetSize()!
 */
inline int kseek(KFILE* file, long offset, int origin);

#include "kseek.inl"
#endif //KIFSTREAM_H
