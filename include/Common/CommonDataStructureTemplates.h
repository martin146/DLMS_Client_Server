/**
 * @file CommonDataStructureTemplates.h
 * @author Filip Grepl
 * @brief Definition of 'CommonDataStructureTemplates' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_COMMONDATASTRUCTURETEMPLATES_H
#define DLMS_CLIENT_SERVER_COMMONDATASTRUCTURETEMPLATES_H

// Use GuruxCore enumerators
#include "GURUX_DLMS/enums/enums.h"
#include "GURUX_DLMS/GXBytebuffer.h"

#include <string>
#include <chrono>

typedef uint8_t BYTE;

/**
 * @brief Delimiter between parameter and value
 *
 */
static char CONFIG_DELIMITER = (char)'=';
/**
 * @brief Comment sequence
 *
 */
static char COMMENT_CHAR = (char)'/';
/**
 * @brief Size system title.
 *
 */
const unsigned short SYS_TITLE_SIZE = 8;
/**
 * @brief Size of auth. key.
 *
 */
const unsigned short AUTH_KEY_SIZE = 16;
/**
 * @brief Size of block cipher key.
 *
 */
const unsigned short BLOCK_CIPHER_KEY_SIZE = 16;
/**
 * @brief Size of dedicated key.
 *
 */
const unsigned short DEDICATED_KEY_SIZE = 16;

/**
 * @brief Preprocessor macro for get variable name
 *
 */
#define GET_VAR_NAME(name) GetVarName(#name)
/**
 * @brief Get the Var Name
 *
 * @param name
 * @return const char*
 */
inline const char* GetVarName(const char* name)
{
    return name;
}

/**
 * @brief General enumerator
 *
 */
enum GENERAL
{
    N_OK,
    OK,
    GENERAL_ERROR,

    NUMBER_GENERAL
};

/**
 * @brief Error codes used in config parser.
 *
 */
enum CONFIG_ERROR_CODES
{
    CONFIG_OK,								// ConfigClient upload OK
    COMMENT,                                // Comment
    CONFIG_ERROR,							// General config error
    INVALID_VALUE,							// Invalid value of parameter at line in config file
    UNKNOWN_PARAM,							// Unknown config parameter at line in config file
    DEVICE_LIST_NOT_FOUND,					// List of servers not found
    DEVICE_FILE_NOT_FOUND,					// Server config file listed in List of servers but not found
    DEVICE_COULD_NOT_BE_LINKED_TO_GURUX,	// Error in linking to GuruxCore
    CLIENT_CONFIG_FILE_NOT_FOUND,			// Client config file not found
    COULD_NOT_CREATE_INSTANCE,				// Constructor error
    CONFIG_SKIP,

    NUMBER_CONFIG_ERROR_CODES = 11
};

/**
 * @brief Socket related user enumerator
 * @brief Decide which transport protocol use
 */
enum IP_TRANSPORT_PROTOCOL
{
    TCP,
    UDP,

    NUMBER_IP_TRANSPORT_PROTOCOL
};

enum DLMS_IEC_PROTOCOL
{
    DLMS,
    IEC,

    NUMBER_DLMS_IEC_PROTOCOL
};

/**
 * @brief ConfigClient parameters used in GuruxCore
 * @brief Taken from Gurux Director
 */
struct MaxPDUSize_HDLC
{
    unsigned int transmit = 128;	// Max payload size in transit for HDLC frame
    unsigned int receive = 128;	// Min payload size in transit for HDLC frame
};

/**
 * @brief ConfigClient parameters used in GuruxCore
 * @brief Taken from Gurux Director
 * @brief Not sure with usage
 */
struct WindowSize_HDLC
{
    unsigned int transmit = 1;
    unsigned int receive = 1;
};

struct Security
{
    DLMS_AUTHENTICATION authentication = DLMS_AUTHENTICATION_NONE;		    // Authentication level of target server
    DLMS_SECURITY_POLICY1 securityPolicy1 = DLMS_SECURITY_POLICY1_NOTHING;	// Security level of target server
    std::string password = "12345678";									    // Password of target server
    unsigned int invocationCounter = 0;

    BYTE userID = 0x00;
    std::string sysTitle;
    std::string authenticationKey;
    std::string blockCipherKey;
    std::string dedicatedKey;
};

/**
 * @brief Get time-since-epoch right now [s]
 *
 * @return long long
 */
inline long long GetNow()
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

#endif //DLMS_CLIENT_SERVER_COMMONDATASTRUCTURETEMPLATES_H
