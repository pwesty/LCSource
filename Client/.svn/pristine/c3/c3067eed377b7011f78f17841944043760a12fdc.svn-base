1506

%{
#include "StdH.h"
#include "Models/Items/ItemHolder/ItemHolder.h"
%}
uses "EntitiesMP/Item";
uses "EntitiesMP/BasicEffects";

// Touch Field Type
enum ItemObjType
{
	0 ITEMOBJ_DEFAULT	"Default Item Object",
	1 ITEMOBJ_RAID		"Raid type Item Object",
};

%{

%}

class CItemObjects : CItem { 
name      "ItemObject";
thumbnail "Mods\\ItemObjects.tbn";
features  "HasName", "IsTargetable";	

properties:
	1 CTFileName m_fnModel      "Model file(.smc)" 'M' = CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc"),
	2 CTString	m_strAnimation	"Model Ani" = "",
	3 BOOL m_bActive			"Active" 'A' = TRUE,
	5 FLOAT m_fStretch			= 1.0f,
	6 INDEX m_iCollectItem		"Collect Item" = -1,
	50 CSoundObject m_soDrop,  
	105 INDEX idItem_NormalBox	"Collision Box"		= -1,
	120 enum ItemObjType eType	 "Item Obj Type" = ITEMOBJ_DEFAULT,
	200	CTString	m_strName	" Name" = "Item",	

components:	
    0 class   CLASS_BASE        "Classes\\Item.ecl",
functions:

	void Precache(void) {
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
		if(en_pmiModelInstance->mi_pmisSerial
		&& en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance
		&& en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance->m_fnmAnimEffectFile != "")
		{
			en_pmiModelInstance->ReadAnimEffectFile(en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance->m_fnmAnimEffectFile);
		}
		GetModelInstance()->RefreshTagManager();

		if( m_strAnimation != "")
		{
			INDEX iAnim = ska_GetIDFromStringTable(m_strAnimation);
			INDEX iDummy1, iDummy2;
			if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
				NewClearState(0.05f);
				ULONG ulFlags = AN_LOOPING;
				AddAnimation(iAnim, ulFlags, 1, 1);            
				OffSetAnimationQueue(FRnd()*10.0f);        
			} else {
				CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
					m_strAnimation, GetName());
			}
		}
	}

	void RenderParticles(void) {
		if (m_bActive)
		{
			Particles_Stardust(this, 1.0f*0.75f, 0.75f*0.75f, PT_STAR08, 128);
		}
	}

	void SetNormalBox(const char* BoxName)
	{
		idItem_NormalBox = ska_GetIDFromStringTable(BoxName);
	}	



procedures:
	Main() 
	{	
		/*if(GetModelInstance() == NULL)
		{ 
			InitAsSkaEditorModel();
			SetSkaModel("Data\\Models\\Editor\\Ska\\Axis.smc");
			return EEnd();
		}*/
		if (m_fnModel=="") {
			m_fnModel=CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc");
		}

		InitAsSkaModel();
		
		BOOL bLoadOK = TRUE;
		// try to load the model
		try {
			SetSkaModel_t(m_fnModel);
			// if failed
		} catch(char *) {            
			bLoadOK = FALSE;
			// set colision info for default model
			//SetSkaColisionInfo();
		}
		if (!bLoadOK) {
			SetSkaModel(CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc"));
			WarningMessage(TRANS("Cannot load ska model '%s'"), (CTString&)m_fnModel);
		}
			
//		SetFlags(GetFlags()|ENF_ITEM);					
//		SetFlags(GetFlags()|ENF_ALIVE);			

		// declare yourself as a model		
		//SetCollisionFlags(ECF_MODEL);	
		//SetPhysicsFlags(EPF_MOVABLE | GetPhysicsFlags());
		//SetCollisionFlags  (ECF_MODEL_NO_COLLISION );
		SetFlags(GetFlags()|ENF_SEETHROUGH);
		SetPhysicsFlags(EPF_MODEL_ITEM);
		SetCollisionFlags(ECF_ITEM);
		
		GetModelInstance()->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));
		GetModelInstance()->mi_bRenderShadow = TRUE;		

		SetDesiredTranslation(FLOAT3D(0,0,0));

//		m_soDrop.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
//		PlaySound(m_soDrop, SOUND_DROP, SOF_3D);

	//	ModelChangeNotify();
		//MessageBox(NULL, "Item Loop", "ItemLoop", MB_OK);
		jump CItem::ItemLoop();
		return;
	};

	ItemCollected(EPass epass) : CItem::ItemCollected {
		ASSERT(epass.penOther!=NULL);
		switch(eType)
		{
		case ITEMOBJ_RAID:
			{ // TAKE_ITEM
				if (m_bActive)
				{
					_pNetwork->SendRaidScene(2, en_ulID, m_iCollectItem);
					m_bActive = FALSE;
				}
			}
			break;
		}
		return;
	};

	ItemActivate(EActivate eAct) : CItem::ItemActivate {
		SetFlagOn(ENF_PROPSCHANGED);
		SwitchToModel();
		SetCollisionFlags(ECF_ITEM);
		m_bActive = TRUE;
		if (m_strAnimation != "")
		{
			INDEX iAnim = ska_GetIDFromStringTable(m_strAnimation);
			AddAnimation(iAnim, AN_LOOPING, 1, 1);
		}
		return;
	};

	ItemDeActivate(EDeactivate eDeAct) : CItem::ItemDeActivate {
		SetFlagOn(ENF_PROPSCHANGED);
		SwitchToEditorModel();
		SetCollisionFlags(ECF_IMMATERIAL);
		m_bActive = FALSE;
		GetModelInstance()->StopAllAnimations(0.5f);
		return;
	};
};
