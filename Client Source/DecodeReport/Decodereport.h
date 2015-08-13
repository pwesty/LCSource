#ifndef SE_INCL_DECODEREPORT_H
#define SE_INCL_DECODEREPORT_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

struct AddressMapping {
  ULONG am_ulSection;
  ULONG am_ulOffset;
  CTString am_strFunction;
};

struct MapFile {
  CStaticStackArray<AddressMapping> mf_aamAddresses;
  CTFileName mf_strImage;
};

#endif //#ifndef SE_INCL_DECODEREPORT_H
