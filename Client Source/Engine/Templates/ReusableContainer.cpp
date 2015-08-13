#ifndef SE_INCL_REUSABLECONTAINER_CPP
#define SE_INCL_REUSABLECONTAINER_CPP
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Templates/ReusableContainer.h>

// Constructor (must specify size of container)
template<class Type>
CReusableContainer<Type>::CReusableContainer(INDEX ctMaxAllocated)
{
	ASSERT(ctMaxAllocated>0);

	rc_iCreateCalls = 0;
	rc_iDeleteCalls = 0;
	rc_ctOwerfows = 0;
	rc_ctMaxOwerflows = 0;
	rc_ctMaxAllocated = ctMaxAllocated;
	rc_bNowDeleting = FALSE;
}

// Destructor
template<class Type>
CReusableContainer<Type>::~CReusableContainer(void)
{
	CObjectRestore<BOOL> or(rc_bNowDeleting,TRUE);
	INDEX ctt = rc_atObjects.Count();
	for(INDEX it=0;it<ctt;it++) {
		Type *pType = &rc_atObjects[rc_atObjects.Count()-1];
		ASSERT(pType!=NULL);
		delete pType;
		rc_atObjects.Remove(pType);
	}
}

// Create new object (uses old object if any is free)
template<class Type>
Type *CReusableContainer<Type>::CreateObject(void)
{
	rc_iCreateCalls++;

	const INDEX ctFreeObjects = rc_aiFreeObjects.Count();
	// If there are some free objects in array
	if(ctFreeObjects>0) {
		// Use free object instead of allocating new one
		const INDEX iFreeObject = rc_aiFreeObjects[ctFreeObjects-1];
		// This object is no longer free
		rc_aiFreeObjects.Pop();
		// Get object
		Type *ptObject = &rc_atObjects[iFreeObject];
		ASSERT(ptObject!=NULL);

#if MEMORY_TRACKING
		// remove deleted flag from object
		ULONG ulFlags = MEMTRACK_GETBLOCKFLAGS(ptObject);
		ulFlags &= ~MTF_NOSTACKTRACE;
		MEMTRACK_SETBLOCKFLAGS(ptObject,ulFlags);
#endif

		// Clear object
		ptObject->Clear();
		// Return object
		return ptObject;
	// if not
	} else {
		// if object is out of allocation array
		if(rc_atObjects.Count()>=rc_ctMaxAllocated) {
			// increase count of owerflow objects
			rc_ctOwerfows++;
			rc_ctMaxOwerflows = Max(rc_ctOwerfows,rc_ctMaxOwerflows);
		}
		// Allocate new object
		Type *ptNew = new Type;
		ASSERT(ptNew!=NULL);
		// Add it to array of allocated objects
		rc_atObjects.Add(ptNew);
		// Return cleared object
		ptNew->Clear();
		return ptNew;
	}
}

// Deletes object (marks object as free)
template<class Type>
void CReusableContainer<Type>::DeleteObject(Type *ptObject)
{
	rc_iDeleteCalls++;

	ASSERT(ptObject!=NULL); // must be valid

	// find object in array
	INDEX ctt = rc_atObjects.Count();
	for(INDEX it=0;it<ctt;it++) {
		const Type *ptFound = &rc_atObjects[it];
		ASSERT(ptFound!=NULL); // must be valid
		// if object has been found
		if(ptFound==ptObject) {
#if _DEBUG
			// Verify that this object wasn't allready deleted
			INDEX ctft = rc_aiFreeObjects.Count();
			for(INDEX ift=0;ift<ctft;ift++) {
				ASSERT(rc_aiFreeObjects[ift]!=it);
			}
#endif
			// Clear object to be sure that no one uses it
			ptObject->Clear();
			// if object fits into allocated array
			if(it<rc_ctMaxAllocated) {
				// Mark object as free
				INDEX &iFree = rc_aiFreeObjects.Push();
				iFree = it;
#if MEMORY_TRACKING
				// mark object as deleted
				ULONG ulFlags = MEMTRACK_GETBLOCKFLAGS(ptObject);
				ulFlags |= MTF_NOSTACKTRACE;
				MEMTRACK_SETBLOCKFLAGS(ptObject,ulFlags);
#endif
				// we are done
				return;
			// if object was allocated out of array
			} else {
				// simply delete object
				rc_atObjects.Remove(ptObject);
				delete ptObject;
				// decrease owerflow count
				rc_ctOwerfows--;
				// done
				return;
			}
		}
	}

	ASSERTALWAYS("Deleted objects wasn't created by CreateObject func");
}

#endif // SE_INCL_REUSABLECONTAINER_CPP
