/** @class TRequestId "TRequestId.h"
 *  @author M.W.A.Roza
 *
 *  Used when identifying request results from a Kalydo Player.
 */

#ifdef KALYDO
#ifndef TREQUESTID_H
#define TREQUESTID_H

typedef unsigned int TRequestType;
const TRequestType RT_USERINFO = 0;
const TRequestType RT_PAYMENT = 1;
const TRequestType RT_PACKAGES = 2;

struct TRequestId
{
	TRequestType requestType;
};

#endif
#endif