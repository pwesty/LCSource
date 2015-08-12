#include "StdH.h"
#include "attendance.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Common/Packet/ptype_attendance.h>
#include <Engine/Contents/Base/ChattingUI.h>

Attendance::Attendance()
{
	m_nAccDate = 0;
}

void Attendance::RecvAttendanceMsg( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pTypeThirdBase* pPacket = reinterpret_cast<pTypeThirdBase*>(istr->GetBuffer());
	CTString		strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	switch(pPacket->thirdType)
	{
	case MSG_SUB_ATTENDANCE_CHECK_RES: // 누적 출석일수 확인 (랜디를 통해 확인 가능)
		{
			ResponseClient::AttendanceCheck* pRecv = reinterpret_cast<ResponseClient::AttendanceCheck*>(istr->GetBuffer());

			m_nAccDate = pRecv->acc_count;

			if( pUIManager->DoesMessageBoxLExist(MSGLCMD_ATTENDANCE_SYSTEM))
				pUIManager->CloseMessageBoxL(MSGLCMD_ATTENDANCE_SYSTEM);

			pUIManager->CreateMessageBoxL( _S(5017, "출석 현황 보기"), UI_QUEST, MSGLCMD_ATTENDANCE_SYSTEM );
			strMessage.PrintF( _S(5018, "[%s]님의 출석현황을 알려드리겠습니다."), _pNetwork->MyCharacterInfo.name );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5019, "현재 %d일째 연속 출석중입니다."), m_nAccDate );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage, -1, 0x6BD2FFFF );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _S(5020, "연속출석 일수에 따라 보상품이 다르니 열심히 출석체크하시길 바랍니다." ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _s(" " ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _S(1143, "보상내용" ) );
			strMessage.PrintF( _S(5021, "%d일연속 출석 : %d%% 추가 경험치"), 2, 3 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5021, "%d일연속 출석 : %d%% 추가 경험치"), 3, 4 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5021, "%d일연속 출석 : %d%% 추가 경험치"), 4, 5 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5021, "%d일연속 출석 : %d%% 추가 경험치"), 5, 10 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5583, "%d일연속 출석 : 호칭 아이템"), 50 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5584, "%d일연속 출석 : 장비 아이템"), 100 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );

			strMessage.PrintF( _S( 5584, "%d일연속 출석 : 장비 아이템"), 150 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5583, "%d일연속 출석 : 호칭 아이템"), 200 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );

			strMessage.PrintF( _S( 5584, "%d일연속 출석 : 장비 아이템"), 250 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5584, "%d일연속 출석 : 장비 아이템"), 300 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );

			strMessage.PrintF( _S( 5583, "%d일연속 출석 : 호칭 아이템"), 365 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _S( 5585, "(365일 보상을 받으시면, 연속출석이 0일로 초기화 됩니다.)") );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, FALSE, _S(1839, "아이템 지급"), 0 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, FALSE, _S(191, "확인") );
		}
		break;
	case MSG_SUB_ATTENDANCE_REWARD_RES: // 누적 일수별 보상
		{
			ResponseClient::AttendanceReward* pRecv = reinterpret_cast<ResponseClient::AttendanceReward*>(istr->GetBuffer());

			switch(pRecv->error_code)
			{
			case ATTENDANCE_ERROR_SUCCESS: // 보상 성공
				break;
			case ATTENDANCE_ERROR_NO_REWARD: // 받을 보상이 없음
				{
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( _S( 674, "이벤트 아이템을 받을 수 없습니다." ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case ATTENDANCE_ERROR_INVEN_FULL: // 인벤토리가 가득 참
				{
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( _S( 675, "인벤토리 공간이 부족해서 이벤트 아이템을 받을 수 없습니다." ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;

	case MSG_SUB_ATTENDANCE_FINISH: // 출석 완료
		{
			UpdateClient::AttendanceFinish* pRecv = reinterpret_cast<UpdateClient::AttendanceFinish*>(istr->GetBuffer());

			m_nAccDate = pRecv->acc_count;

			strMessage.PrintF(_S(5016, "출석체크가 되었습니다. 연속 출석 %d일째 하고 계십니다."), m_nAccDate );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);
			pUIManager->GetChattingUI()->AddSysMessage( _S(5024, "자정이 지나면 재접속을 하셔야 새롭게 출석이 인정됩니다."), SYSMSG_NORMAL);
			strMessage.PrintF(_S(5025, "출석 일수에 따라 +%d의 추가 경험치를 받습니다."), pRecv->up_exp );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);

			if ( m_nAccDate == 100 || m_nAccDate == 200 )
			{
				MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 5117, "연속 출석 일수에 따른 보상을 쥬노 란돌마을의 랜디에게 받으시길 바랍니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;
	case MSG_SUB_ATTENDANCE_INFO_UPD: // 게임 접속 시,
		{
			UpdateClient::AttendanceInfo* pRecv = reinterpret_cast<UpdateClient::AttendanceInfo*>(istr->GetBuffer());

			m_nAccDate = pRecv->acc_count;

			strMessage.PrintF(_S(5015, "연속 출석 %d일째 하고 계십니다."), m_nAccDate);
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);
			pUIManager->GetChattingUI()->AddSysMessage( _S(5022, "연속 출석 일수에 따라 추가 경험치를 획득하실 수 있습니다."), SYSMSG_NORMAL);
			strMessage.PrintF(_S(5023, "출석 일수에 따라 +%d%%의 추가 경험치를 받습니다."), pRecv->up_exp);
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);
		}
		break;
	case MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_UPD:
		{
			UpdateClient::AttendanceAssureOk* pRecv = reinterpret_cast<UpdateClient::AttendanceAssureOk*>(istr->GetBuffer());

			if ( pUIManager->DoesMessageBoxExist(MSGCMD_ATTENDANCE_ASSURE_OK_UPD) == TRUE )
				pUIManager->CloseMessageBox(MSGCMD_ATTENDANCE_ASSURE_OK_UPD);

			strMessage.PrintF(_S(6315, "현재 출석 유지 보험이 적용 중입니다. 출석 체크를 하시려면 출석 유지 보험을 해제하여야 합니다. 정말 출석 유지 보험을 해제 하시겠습니까?"));
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_YESNO, UI_NONE, MSGCMD_ATTENDANCE_ASSURE_OK_UPD);
			MsgBoxInfo.AddString(strMessage);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;

	case MSG_SUB_ATTENDANCE_SYS_MSG:
		{
			UpdateClient::AttendanceSysMsg* pRecv = reinterpret_cast<UpdateClient::AttendanceSysMsg*>(istr->GetBuffer());
			
			switch(pRecv->sysMsg)
			{
			case ATTENDANCE_SYS_MSG_END_OK:
				strMessage.PrintF(_S(6319, "출석 유지 보험이 해제되었습니다. 현재 연속출석 %d 일째이며, 금일 출석 체크를 완료하셔야 연속출석이 유지됩니다."), m_nAccDate );
				break;
			case ATTENDANCE_SYS_MSG_TIME_END:
				strMessage.PrintF(_S(6318, "출석 유지 보험의 사용 기간이 만료되었습니다."));
				break;
			case ATTENDANCE_SYS_MSG_TIME_END_INIT_ATTENDANCE:
				strMessage.PrintF(_S(6320, "출석 유지 보험이 만료된 후 연속 출석을 하지 않아 출석 일 수가 초기화 되었습니다."));
				break;
			}
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL, 0xFF0000FF);
		}
		break;
	}
}
