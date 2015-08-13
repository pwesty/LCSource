1505

%{
#include "StdH.h"
#include "Models/Items/ItemHolder/ItemHolder.h"
%}

uses "EntitiesMP/BasicEffects";


%{

%}

class CViewingItems : CMovableModelEntity { 
name      "Item";
thumbnail "Mods\\test.tbn";
features  "HasName", "IsTargetable";

properties:

	5 FLOAT m_fStretch			= 1.0f,
	50 CSoundObject m_soDrop,  
	105 INDEX idItem_NormalBox	"Collision Box"		= -1,
	200	CTString	m_strName	" Name" = "Item",	

components:	
    0 class   CLASS_BASE        "Classes\\Item.ecl",

//	50 sound   SOUND_DROP		"Data\\Sounds\\etc\\a_item_drop.wav",
//	51 sound   SOUND_MONEY_DROP		"Data\\Sounds\\etc\\a_money_drop.wav",
functions:

	void SetNormalBox(const char* BoxName)
	{
		idItem_NormalBox		= ska_GetIDFromStringTable(BoxName);
	}	



procedures:

	Main() 
	{	
		if(GetModelInstance() == NULL)
		{ 
			InitAsSkaEditorModel();
			SetSkaModel("Data\\Models\\Editor\\Ska\\Axis.smc");
			return EEnd();
		}		
			
		SetFlags(GetFlags()|ENF_ITEM);					
//		SetFlags(GetFlags()|ENF_ALIVE);			

		// declare yourself as a model		
		//SetCollisionFlags(ECF_MODEL);	
		SetPhysicsFlags(EPF_MOVABLE | GetPhysicsFlags());
		SetCollisionFlags  (ECF_MODEL_NO_COLLISION );
		
		
		GetModelInstance()->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));
		GetModelInstance()->mi_bRenderShadow = TRUE;		

		SetDesiredTranslation(FLOAT3D(0,0,0));


//		m_soDrop.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
//		PlaySound(m_soDrop, SOUND_DROP, SOF_3D);

	//	ModelChangeNotify();
		return;
	};

};
