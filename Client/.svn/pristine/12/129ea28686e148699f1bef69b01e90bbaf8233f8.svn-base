// ----------------------------------------------------------------------------
//  File : PetInfo.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------
#ifndef	PETINFO_H_
#define	PETINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/FileName.h>

enum ePetAnimation
{
	PET_ANIM_START = 0, 
	PET_ANIM_WALK_1 = PET_ANIM_START,
	PET_ANIM_IDLE_1,
	PET_ANIM_IDLE_2,
	PET_ANIM_RUN_1,
	PET_ANIM_PICK,
	PET_ANIM_DAMAGE,
	PET_ANIM_DIE,
	PET_ANIM_LEVELUP,
	PET_ANIM_ATTACK,
	PET_ANIM_SKILL_0,
	PET_ANIM_SKILL_1,
	PET_ANIM_SKILL_2,
	PET_ANIM_SKILL_3,
	PET_ANIM_SKILL_4,
	PET_ANIM_SKILL_0_01,
	PET_ANIM_SKILL_0_02,
	PET_ANIM_END = PET_ANIM_SKILL_0_02,
	PET_ANIM_TOTAL,
};

enum ePetAttackType
{
	PET_ATTACK1 = 0,
	PET_ATTACK2,
	PET_ATTACK3,
	PET_ATTACK4,
	PET_MAX_ATTACK,
};

enum ePetAge
{
	HORSE_CHILD	= 0,	// 말
	HORSE_BOY,
	HORSE_ADULT,	
	DRAGON_CHILD,		// 용
	DRAGON_BOY,
	DRAGON_ADULT,
	
	BLUE_HORSE_CHILD	= 6,	// 파란 말
	BLUE_HORSE_BOY,
	BLUE_HORSE_ADULT,	
	PINK_DRAGON_CHILD,			// 핑크 용
	PINK_DRAGON_BOY,
	PINK_DRAGON_ADULT,

	MISTERY_HORSE_CHILD	= 12,	// 불가사의한 말
	MISTERY_HORSE_BOY,
	MISTERY_HORSE_ADULT,	
	MISTERY_DRAGON_CHILD,		// 불가사의한 용
	MISTERY_DRAGON_BOY,
	MISTERY_DRAGON_ADULT,

	TOTAL_PET,
};

//---------------------------------------------------------------------
#define MAX_PET_AGE		(3)
class ENGINE_API CPetInfo
{
private:
	typedef struct _tagPetInfo
	{
		_tagPetInfo()
		{			
			bRide						= FALSE;
			fRunSpeed					= 0.0f;
			fWalkSpeed					= 0.0f;
		};

		~_tagPetInfo()
		{
		};

		CTString		strName;									// 이름 정보.
		CTString		strFileName;								// 파일명.
		CTString		aStrAnimationName[PET_ANIM_TOTAL];			// 애니메이션 이름.		
		BOOL			bRide;
		FLOAT			fWalkSpeed;
		FLOAT			fRunSpeed;
		
	}sPetInfo;
	
public:

	enum ePetType
	{
		PET_HORSE	= 0,
		PET_DRAGON,
	};
	
	CPetInfo();
	~CPetInfo();

	static CPetInfo &Instance()			{ return m_instance;	}

	// Set Properties
	void		SetName( int iPet, const CTString& strName );

	// Get Properties
	const CTString	&GetName( int iPet, int iAge ) const;
	const CTString	&GetFileName( int iPet, int iAge ) const;

	// [070824: Su-won] PET_COLOR_CHANGE
	const CTString &GetColoredTexFileName( int iPet, int iAge, int iColor ) const;
	
	const CTString	&GetAnimationName( int iPet, int iAnimType ) const;
	void		SetPetDataToEntity( int iPet, int iAge, CEntity *pEntity, BOOL bUseAI = FALSE );

	FLOAT		GetWalkSpeed( int iPet, int iAge ) const;
	FLOAT		GetRunSpeed( int iPet, int iAge ) const;

	BOOL		IsRide( int iPet, int iAge );

protected:
	static CPetInfo m_instance;
	sPetInfo		*m_pPetInfo;
};

inline CPetInfo &PetInfo()				{ return CPetInfo::Instance();	}

#endif // PETINFO_H_