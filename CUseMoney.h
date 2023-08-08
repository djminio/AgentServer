
#ifndef _CUSEMONEY_H
#define _CUSEMONEY_H

/*
class CWSASocket
{
public:
	CWSASocket():m_bServer(false),m_sSocket(0){};
	CwSASocket( bool bServer, SOCKET sSocket ):m_bServer(bServer),m_sSocket(sSocket) {};
	~CWSASocket(){};

	
private:
	bool m_bServer;	//�������� Ŭ���̾�Ʈ����
	SOCKET	m_sSocket;

};
*/

//#include <winsock2.h>

//#include <string>

#include "stdafx.h"
#include "usertable.h"
#include <list>
#include <algorithm>
using namespace std;

#define WM_USER_ALIVE_PAY WM_USER+101 // 030701 kyo

#pragma pack(push,1)
typedef struct userinfo_pay
	{
		//Agent����
		unsigned long	dwConnectionIndex;
		char szUserPass[32];
		//char	szName[21];

		//PayServer����
		//unsigned long	dwGameId;
		char	szGameId[32];
		char	szUserId[32];
		char	szUserType[32];
		//unsigned long	dwUserType;
		//char szGumUser[3][21];
		int		nAliveTime;	//030703 YGI

	}USERINFOPAY;
#pragma pack (pop)


class CAsyncClient//	: public CWSASocket
{
enum eMSG_TYPE
{
	//WM_SOCKET_EVENT		= 0x0400+100,
	MAX_RECV_BUF_SIZE	= 128,

	GUM_MSG_DIVISION_HEAD	= 0x1D,
	GUM_MSG_DIVISION_TAB	= 0x09,
	GUM_MSG_DIVISION_NULL	= 0x00,

	GUM_STATE_INIT			= 0,	
	GUM_STATE_RUNNING		= 1,
	GUM_STATE_CLOSE			= 2,

	//seq = 0
	GUM_MSG_INITALIZED			= 1,
	GUM_MSG_INIT_FAIL			= 88,
	GUM_MSG_MESSAGEFORM_WRONG	= 99,
	GUM_MSG_RESPONS_INITALIZED	= 100,

	//seq = 1
	GUM_MSG_LOGIN_ACCESS		= 10,
	GUM_MSG_LOGIN_SUCCESS		= 11,
	GUM_MSG_WRONG_PASS			= 12,
	GUM_MSG_HAVENO_AUTHORITY	= 13,
	GUM_MSG_END_OF_DATE			= 14,
	GUM_MSG_ALREADY_LOGIN		= 15,
	GUM_MSG_ALL_SPEND_IP		= 16,
	GUM_MSG_DISCONNECT_USER		= 17,
	GUM_MSG_END_OF_DATE_1DAY	= 18,
	GUM_MSG_END_OF_DATE_1DAY2	= 19,	

	GUM_MSG_LOGOUT_ACCESS		= 20,
	GUM_MSG_LOGOUT_SUCCESS		= 21,
	GUM_MSG_LOGOUT_FAIL			= 22,

	GUM_MSG_ALIVE_USER			= 30,	//030703 YGI

	//seq =-1	
	GUM_MSG_USER_TIMEOVER		= 51,
	GUM_MSG_USER_TIMEOVER_1DAY	= 52,
	GUM_MSG_USER_TIMEOVER_1DAY2	= 53,
	GUM_MSG_USER_DIS_TIMEOVER	= 61,
	GUM_MSG_USER_DIS_TIMEOV_1D	= 62,
	GUM_MSG_USER_DIS_TIMEOV_1D2	= 63,
	GUM_MSG_USER_DIS_OUSER_REQ	= 64,	//�ٸ� ������ �䱸�� 
	
}EMSG_TYPE;

//typedef
typedef list<USERINFOPAY>			LISTUSER;
typedef list<USERINFOPAY>::iterator	LISTUSERIT;
public:
static const char* SERVER_NAME;
static const char* GAME_ID;
static const char* USER_TYPE;

public:
	CAsyncClient();
	CAsyncClient( const char* strName, const int nGrup, const char* szServerNum, const int nZone);
	~CAsyncClient();

	SOCKET	InitSocket( const char* strIP, const int nPort );		//
	int		InitConnectPayServer();						// ���ݼ����� Initialized Message�� �ְ� �޴´�.
	void	CloseConnection();
	int		SendInitialMsg2PayServer();					
	void	SetAgentInfo( const char* szName, const int nGroup, const char* szServerNum, const int nZone );
	void	SetUserInfo( USERINFOPAY& info, const int nCn, const char* szUserId, const char* szUserPass, const char* szGameId, const char* szUserType );

	//state
	void	SetCurrentState( const int nState ) { m_nState = nState;};
	int		GetCurrentState( ) const 
				{ 
					return m_nState; 
				};
	bool	IsRunning()	const 
				{ 
					if( GUM_STATE_RUNNING == GetCurrentState() ) return true; 
					return false;
				};
	//Analysys
	int		AnalysysMessage( char* szMsg );			// ���� �޽����� �м��ؼ� �����Ѵ�.
	int		AnalysysMsgHeader( char* szMsg );			//Seq�����ϱ�
	char*	AnalysysMsgHeader( char* szMsg, int& nSeq );	//Msgbody�κи� �����ϱ�
	char*	AnalysysMsgType( char* szMsgBody, int& nType );	
	
	int		AnalysysMsgBody( char* szMsgBody , USERINFOPAY& userInfo, int& nRestTime);	// analysys Body
	//int		AnalysysMsgBody_User( char* szMsgBody , USERINFO& userInfo, int& nRestTime)

	HWND	CreateMsgWindow( WNDPROC messageproc);
	int		AddPayUser( USERINFOPAY& stUser );					// list�� ���� ����
	void	DisconnectUser( USERINFOPAY& stUser );				
	bool	DeletePayUser( USERINFOPAY& stUser );				// list���� ���� ����
	list<USERINFOPAY>::iterator
			FindUserIterator( const char* szUserName);			// list���� �ش� ������ �ݺ��� ����

	//030703 YGI
	void	CheckAliveUser();
	void	SendAliveMessage( USERINFOPAY& userInfo );

	void	SendMsgToPayServer( int nType , USERINFOPAY& stUser);	//���Ŀ� ���� ������ �����ش�. 
	int		SendPayFormatMessage( char* strMsg );				// GUM���� ������ ���Ŀ� �´� ���ڿ��� send�Ѵ�.
	void	SendKeepaliveMsg();									//���� �޽���. 1�а������� �������� �Ѵ�. 
	void	SendUserLogin( USERINFOPAY& userInfo, const char* szRIP, const char* szVIP , const int nReConnect);
	void	SendUserLogout( USERINFOPAY& userInfo);				//�α׾ƿ��ش޶�
	void	RecvUserLogin( USERINFOPAY& userInfo);				// �α��� ����
	void	RecvUserLoginFail( USERINFOPAY& userInfo, int nType);	//�α��� ����
	void	RecvUserLogout( USERINFOPAY& userInfo);					//�α׾ƿ� ��û
	void	RecvUserLogoutFail( USERINFOPAY& userInfo );			//�α׾ƿ�����
	void	RecvUserAlert( USERINFOPAY& userInfo, int nTime);		//���� ���޽���
	void	RecvUserDisconnect( USERINFOPAY& userInfo, int nMsgType);	//���� ���� ���� ����	
	void 	RecvResponsMessage( SOCKET socket , char* szBuf); 			//���ۿ��� �޽����� �д´�. 
	USERINFO*	FindPayUser( const char* szUserId);	

	//DB connect
	void	SendUserLoginSuccess2DB( const DWORD dwConnectionIndex, const char* szUserId );
	//Client connect
	void	SendUser2Client( const DWORD dwConnectionIndex, const char* szUserId, const int nType);	

	void	LoginUser( const int nCn, const char* szUserId, const char* szUserPass , const char* szGameId, const char* szUserType, const char* szUserRealIp, const char* szUserVrIp);
	void	LogoutUser( const int nCn, const char* szUserName );
	void	LogoutUser( USERINFO *pUser );					// �ش� ������ ���ݼ��񽺶� ���� �����Ŵ
	void	LogoutAllUser();

	list<USERINFOPAY>	GetAllUser(){ return m_vtUser;	};		//��� ���������� �� �ش�. 
	void	ShowError( int nError );
	bool	IsRightType( int nType );
	
protected:
	//list<USERINFOPAY*> m_vtUser;		//������ȣ ���̺�
	void	SetGumHeader( const char* szHeader, const char* szVer )
				{ 
					sprintf( m_szGumHeader, "%s/%s", szHeader, szVer); 
					sprintf( m_szKeepaliveMsg, "%s\t0\0352\theart_beat\t\0", m_szGumHeader );
				};
//	void	SetDivided( const char* szHeaderDivided, const char* szWordDivided, const char* szNULL ){};
	LISTUSER m_vtUser;		//������ȣ ���̺�
	char	m_szGumHeader[16];	//�ش�����
	char	m_szKeepaliveMsg[128];

private:
	static const char dvHeader;
	static const char dvWord;
	static const char* dvNull;

	SOCKET	m_sSocket;			//���ݼ����� ����ϴ� ��Ĺ
	char	m_strServerName[24];		//�����̸�
	int		m_nServerGroupNum;	//�����׷��ȣ ������ ��ȣ
	char	m_szServerNum[24];			//������ȣ IP
	int		m_nZoneNum;			//�� ��ȣ ��Ʈ��ȣ
	int		m_nState;			//����

	CRITICAL_SECTION		m_cs;
};
extern CAsyncClient g_cAsyncPay;
extern void RecvReponseGUMP( SOCKET socket );
#endif //_CUSEMONEY_H


