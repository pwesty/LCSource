#ifndef SE_INCL_RECT_H
#define SE_INCL_RECT_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Math/Vector.h>

class CTRect
{
public:
  CTRect(void) {
  };
  CTRect(const CTRect &rcOther) {
    rc_slLeft   = rcOther.rc_slLeft;
    rc_slTop    = rcOther.rc_slTop;
    rc_slRight  = rcOther.rc_slRight;
    rc_slBottom = rcOther.rc_slBottom;
  };
  CTRect(CTPoint ptTopLeft, CTPoint ptBottomRight) {
    rc_slLeft   = ptTopLeft(1);
    rc_slTop    = ptTopLeft(2);
    rc_slRight  = ptBottomRight(1);
    rc_slBottom = ptBottomRight(2);
  };
  CTRect(SLONG slLeft, SLONG slTop, SLONG slRight, SLONG slBottom) {
    rc_slLeft   = slLeft;
    rc_slTop    = slTop;
    rc_slRight  = slRight;
    rc_slBottom = slBottom;
  };

  CTPoint GetMin(void) const {
    return CTPoint(rc_slLeft,rc_slTop);
  }
  CTPoint GetMax(void) const {
    return CTPoint(rc_slRight,rc_slBottom);
  }
  SLONG GetWidth(void) const {
    return rc_slRight - rc_slLeft;
  }
  SLONG GetHeight(void) const {
    return rc_slBottom - rc_slTop;
  }
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
  FLOAT GetRadius(void) const
  {
	  if(GetWidth() > GetHeight())	return GetWidth() * 1.414f * 0.5f;
	  else							return GetHeight() * 1.414f * 0.5f;
  }
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
public:
  SLONG rc_slLeft;
  SLONG rc_slTop;
  SLONG rc_slRight;
  SLONG rc_slBottom;
};

extern inline CTRect ClampRect(const CTRect &rc, const CTRect &rcRegion)
{
  CTRect rcResult;
  rcResult.rc_slLeft   = Clamp(rc.rc_slLeft,   rcRegion.rc_slLeft, rcRegion.rc_slRight);
  rcResult.rc_slRight  = Clamp(rc.rc_slRight,  rcRegion.rc_slLeft, rcRegion.rc_slRight);
  rcResult.rc_slTop    = Clamp(rc.rc_slTop,    rcRegion.rc_slTop,  rcRegion.rc_slBottom);
  rcResult.rc_slBottom = Clamp(rc.rc_slBottom, rcRegion.rc_slTop,  rcRegion.rc_slBottom);
  return rcResult;
}

extern inline BOOL RectTouchesRect(const CTRect &rc1, const CTRect &rc2)
{
  if(rc1.rc_slLeft>rc2.rc_slRight || rc1.rc_slRight<rc2.rc_slLeft) {
    return FALSE;
  }
  if(rc1.rc_slTop>rc2.rc_slBottom || rc1.rc_slBottom<rc2.rc_slTop) {
    return FALSE;
  }
  return TRUE;
}

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
class CRectFlt
{
public:
  CRectFlt(void) {
  };
  CRectFlt(const CRectFlt &rcOther)
  {
    m_fLeft   = rcOther.m_fLeft;
    m_fTop    = rcOther.m_fTop;
    m_fRight  = rcOther.m_fRight;
    m_fBottom = rcOther.m_fBottom;
  };
  CRectFlt(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom)
  {
    m_fLeft   = fLeft;
    m_fTop    = fTop;
    m_fRight  = fRight;
    m_fBottom = fBottom;
  };

  FLOAT GetWidth(void) const	{ return m_fRight - m_fLeft; }
  FLOAT GetHeight(void) const	{ return m_fBottom - m_fTop; }
  FLOAT GetRadius(void) const
  {
	  if(GetWidth() > GetHeight())	return GetWidth() * 1.414f * 0.5f;
	  else							return GetHeight() * 1.414f * 0.5f;
  }
  void Normalize()
  {
	  if(m_fLeft > m_fRight)
	  {
		  FLOAT temp = m_fLeft;
		  m_fLeft = m_fRight;
		  m_fRight = temp;
	  }
	  if(m_fTop > m_fBottom)
	  {
		  FLOAT temp = m_fTop;
		  m_fTop = m_fBottom;
		  m_fBottom = temp;
	  }
  }
public:
  FLOAT m_fLeft;
  FLOAT m_fTop;
  FLOAT m_fRight;
  FLOAT m_fBottom;
};
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)


#endif
