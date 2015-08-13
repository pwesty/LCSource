/*
* PreUserAttachment.h
*
*  Created on: 2011. 10. 31.
*      Author: ¿±¿œ
*/

#ifndef PREUSERATTACHMENTBASE_H_
#define PREUSERATTACHMENTBASE_H_

#define MAX_COUNT 255
#define MAX_SOCKET 50
#define BIGINT long long
//#include <string>

class PreUserAttachmentBase
{
public:
	PreUserAttachmentBase() : characterIdx( 0 ), slotNumber( 0 ), preSlot( 0 ), itemidx( 0 ), option1( 0 ), option2( 0 ), option3( 0 ), option4( 0 ),option5( 0 ),
		optionLevel1( 0 ), optionLevel2( 0 ), optionLevel3( 0 ), optionLevel4( 0 ), optionLevel5( 0 ), timeLimite1( 0 ), timeLimite2( 0 ), plus( 0 ),flag( 0 ),
		itemCount( 0 )
	{
		memset( itemSerial, 0x00, sizeof( itemSerial ) );
		memset( socket, 0x00, sizeof( socket ) );
	};
	~PreUserAttachmentBase()
	{
	}

public:

	void setCharacter( int _characterIdx)
	{
		characterIdx = _characterIdx;
	}

	void setSlotNumber( int _slotNumber )
	{
		slotNumber = _slotNumber;
	}

	void setPreSlot( short _preSlot )
	{
		preSlot = _preSlot;
	}

	void setItemSerial( const char *_itemSerial )
	{
		memcpy( itemSerial, _itemSerial, sizeof( itemSerial ) -1 );
	}

	void setItemIdx( int _itemIdx )
	{
		itemidx = _itemIdx;
	}

	void setOption1( short _option1 )
	{
		option1 = _option1;
	}

	void setOption2( short _option2 )
	{
		option2 = _option2;
	}

	void setOption3( short _option3 )
	{
		option3 = _option3;
	}

	void setOption4( short _option4 )
	{
		option4 = _option4;
	}

	void setOption5( short _option5 )
	{
		option5 = _option5;
	}

	void setOptionLevel1( short _optionLevel1 )
	{
		optionLevel1 = _optionLevel1;
	}

	void setOptionLevel2( short _optionLevel2 )
	{
		optionLevel2 = _optionLevel2;
	}

	void setOptionLevel3( short _optionLevel3 )
	{
		optionLevel3 = _optionLevel3;
	}

	void setOptionLevel4( short _optionLevel4 )
	{
		optionLevel4 = _optionLevel4;
	}

	void setOptionLevel5( short _optionLevel5 )
	{
		optionLevel5 = _optionLevel5;
	}

	void setSocket( const char* _socket)
	{
		memcpy( socket, _socket, sizeof( socket ) -1 );
	}

	void setTimeLimite1( int _limit1 )
	{
		timeLimite1 = _limit1;
	}

	void setTimeLimite2( int _limit2 )
	{
		timeLimite2 = _limit2;
	}

	void setPlus( int _plus )
	{
		plus = _plus;
	}

	void setFlag( int _flag )
	{
		flag = _flag;
	}

	void setItemCount( BIGINT _itemCount )
	{
		itemCount = _itemCount;
	}

	int getCharacterIdx()
	{
		return characterIdx;
	}

	int getSlotNumber()
	{
		return slotNumber;
	}

	int getPreSlot()
	{
		return preSlot;
	}

	const char * getItemSerial() const
	{
		return itemSerial;
	}

	int getItemIdx()
	{
		return itemidx;
	}

	short getOption1()
	{
		return option1;
	}

	short getOption2()
	{
		return option2;
	}

	short getOption3()
	{
		return option3;
	}

	short getOption4()
	{
		return option4;
	}

	short getOption5()
	{
		return option5;
	}

	short getOptionLevel1()
	{
		return optionLevel1;
	}

	short getOptionLevel2()
	{
		return optionLevel2;
	}

	short getOptionLevel3()
	{
		return optionLevel3;
	}

	short getOptionLevel4()
	{
		return optionLevel4;
	}

	short getOptionLevel5()
	{
		return optionLevel5;
	}

	const char* getSocket() const
	{
		return socket;
	}

	int getTimeLimite1()
	{
		return timeLimite1;
	}

	int getTimeLimite2()
	{
		return timeLimite2;
	}

	int getPlus()
	{
		return plus;
	}

	int getFlag()
	{
		return flag;
	}

	BIGINT getItemCount()
	{
		return itemCount;
	}

	void clear()
	{
		characterIdx = 0;
		slotNumber = 0;
		preSlot = 0;
		memset( itemSerial, 0x00, sizeof( itemSerial ));
		itemidx = -1;
		option1 = 0;
		option2 = 0;
		option3 = 0;
		option4 = 0;
		option5 = 0;
		optionLevel1 = 0;
		optionLevel2 = 0;
		optionLevel3 = 0;
		optionLevel4 = 0;
		optionLevel5 = 0;
		memset( socket, 0x00, sizeof( socket ));
		timeLimite1 = 0;
		timeLimite2 = 0;
		plus = 0 ;
		flag = 0;
		itemCount = 0;
	}
	void setOriginVar1(int _val)
	{
		originVar1 = _val;
	}
	void setOriginVar2(int _val)
	{
		originVar2 = _val;
	}
	void setOriginVar3(int _val)
	{
		originVar3 = _val;
	}
	void setOriginVar4(int _val)
	{
		originVar4 = _val;
	}
	void setOriginVar5(int _val)
	{
		originVar5 = _val;
	}
	void setOriginVar6(int _val)
	{
		originVar6 = _val;
	}
	int getOriginVar1() const
	{
		return originVar1;
	}
	int getOriginVar2() const
	{
		return originVar2;
	}
	int getOriginVar3() const
	{
		return originVar3;
	}
	int getOriginVar4() const
	{
		return originVar4;
	}
	int getOriginVar5() const
	{
		return originVar5;
	}
	int getOriginVar6() const
	{
		return originVar6;
	}
private:
	int characterIdx;
	int slotNumber;
	short preSlot;
	char itemSerial[MAX_COUNT + 1];
	int itemidx;
	short option1;
	short option2;
	short option3;
	short option4;
	short option5;
	short optionLevel1;
	short optionLevel2;
	short optionLevel3;
	short optionLevel4;
	short optionLevel5;
	char socket[MAX_SOCKET + 1];
	int timeLimite1;
	int timeLimite2;
	int plus;
	int flag;
	BIGINT itemCount;
	int originVar1;
	int originVar2;
	int originVar3;
	int originVar4;
	int originVar5;
	int originVar6;
};

#endif /* PREUSERATTACHMENTBASE_H_ */
//
