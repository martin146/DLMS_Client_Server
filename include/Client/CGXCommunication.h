#ifndef GXCOMMUNICATION_H
#define GXCOMMUNICATION_H

#include "GURUX_DLMS/Client/GXDLMSSecureClient.h"

#ifdef _WIN323
#define _CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#ifdef _WIN32
#include <tchar.h>
#include <winsock.h> //Add support for sockets
#elif __linux__
#define INVALID_HANDLE_VALUE -1
#include <unistd.h>
#include <arpa/inet.h> //Add support for sockets
#include <netdb.h> //Add support for sockets
#include <sys/ioctl.h>
#endif

#include <cstdlib>
#include <ctime>

/**
 * @brief Tato třída propojuje Gurux driver a vztvořenou uživatelskou vrstvu.
 */
class CGXCommunication
{
	CGXDLMSSecureClient* m_Parser;

	int m_socket;

	static const unsigned int RECEIVE_BUFFER_SIZE = 200;

	unsigned char   m_Receivebuff[RECEIVE_BUFFER_SIZE];

	char* m_InvocationCounter;

	std::string parentDevName;

#if defined(_WIN32) || defined(_WIN64)
    HANDLE			m_hComPort;
    OVERLAPPED		m_osWrite;
    OVERLAPPED		m_osReader;
#elif __linux__
    int             m_hComPort;
#endif

	int m_WaitTime;

	int Read(unsigned char eop, CGXByteBuffer& reply);
	int UpdateFrameCounter(void);

protected:


public:

	/**
	 * @brief Getter hlavní Gurux core třídy.
	 */
	CGXDLMSSecureClient* GetGuruxCore_Client(void) { return m_Parser; }

	/**
	 * @brief CGXCommunication contructor.
	 * 
	 */
	CGXCommunication(CGXDLMSSecureClient* pCosem, int wt, char* invocationCounter, std::string devName);
	/**
	 * @brief Destructor.
	 */
	~CGXCommunication(void);

	/**
	 * @brief Ukončení DLMS spojení.
	 * 
	 * @return int 
	 */
	int Disconnect(void);
	/**
	 * @brief Ukončení DLMS spojení a následné ukončení komunikačního kanálu.
	 * 
	 * @return int 
	 */
	int Close(void);
	/**
	 * @brief Navázání TCP/IP komunikace.
	 * 
	 * @return int 
	 */
	int Connect(const char* pAddress, unsigned short port = 4059);

/**
 * @brief Tato funkce vrátí současný čas jako string.
 * 
 */
static inline void Now(std::string& str)
{
	time_t tm1 = time(NULL);
	struct tm dt;
	char tmp[10];
	int ret;

#if _MSC_VER > 1000
    localtime_s(&dt, &tm1);
    ret = sprintf_s(tmp, 10, "%.2d:%.2d:%.2d", dt.tm_hour, dt.tm_min, dt.tm_sec);
#else
    dt = *localtime(&tm1);
    ret = sprintf(tmp, "%.2d:%.2d:%.2d", dt.tm_hour, dt.tm_min, dt.tm_sec);
#endif

	str.append(tmp, ret);
}

	/**
	 * @brief Tato funkce přečte přijatý DLMS packet.
	 *
	 * @return int 
	 */
	int ReadDLMSPacket(CGXByteBuffer& data, CGXReplyData& reply);
	/**
	 * @brief Tato funkce rozparsuje přijatý DLMS packet.
	 * 
	 * @return int 
	 */
	int ReadDataBlock(CGXByteBuffer& data, CGXReplyData& reply);
	/**
	 * @brief Tato funkce rozparsuje přijatý DLMS packet.
	 * 
	 * @param data 
	 * @param reply 
	 * @return int 
	 */
	int ReadDataBlock(std::vector<CGXByteBuffer>& data, CGXReplyData& reply);
	/**
	 * @brief Inicializace DLMS spojení ke SM.
	 * 
	 * @return int 
	 */
	int InitializeConnection(void);
	/**
	 * @brief Tato funkce vyčte association view objekt.
	 * 
	 * @return int 
	 */
	int GetAssociationView(void);

	/**
	 * @brief Tato funkce vyčte ze SM požadovaný objekt.
	 * 
	 * @return int 
	 */
	int Read(CGXDLMSObject* pObject, int attributeIndex, std::string& value);
	/**
	 * @brief Tato funkce vyčte ze SM požadovaný objekt.
	 *
	 * @return int
	 */
	int Read(CGXDLMSObject* pObject, int attributeIndex, std::string& value, DLMS_DATA_TYPE& value_unit);
	/**
	 * @brief Tato funkce vyčte ze SM požadované objekty.
	 *
	 * @return int
	 */
	int ReadList(std::vector<std::pair<CGXDLMSObject*, unsigned char> >& list);

	/**
	 * @brief Tato funkce zapíše hodnotu do požadovaného objektu.
	 *
	 * @return int
	 */
	int Write(CGXDLMSObject* pObject, int attributeIndex, CGXDLMSVariant& value);

	/**
	 * @brief Tato funkce zapíše hodnotu do požadovaného objektu.
	 *
	 * @return int
	 */
	int Write(CGXDLMSObject* pObject, int attributeIndex);


	int Method(CGXDLMSObject* pObject, int ActionIndex, CGXDLMSVariant& value);

	int ReadRowsByRange(CGXDLMSProfileGeneric* pObject, CGXDateTime& start, CGXDateTime& end, CGXDLMSVariant& rows);

	int ReadRowsByRange(CGXDLMSProfileGeneric* pObject, struct tm* start, struct tm* end, CGXDLMSVariant& rows);

	int ReadRowsByEntry(CGXDLMSProfileGeneric* pObject, unsigned int Index, unsigned int Count, CGXDLMSVariant& rows);

	/**
	 * @brief Tato funkce vyčte ze SM Scaler and Units.
	 * 
	 * @return int 
	 */
	int ReadScalerAndUnits(void);
	/**
	 * @brief Tato funkce vyčte ze SM Profile Generic.
	 * 
	 * @return int 
	 */
	int GetProfileGenericColumns(void);
	/**
	 * @brief Tato funkce vyčte ze SM ReadOut.
	 * 
	 * @return int 
	 */
	int GetReadOut(void);
	/**
	 * @brief Tato funkce vyčte ze SM Profile Generics.
	 * 
	 * @return int 
	 */
	int GetProfileGenerics(void);

	/**
	 * @brief Tato funkce vyčte ze SM všechny objekty.
	 * @return int 
	 */
	int ReadAll(void);

};

#endif
