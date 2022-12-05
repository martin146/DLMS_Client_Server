/**
 * @file DataStructureTemplates.h
 * @author Filip Grepl
 * @brief Basic data structures for server app with default values
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DATASTRUCTURETEMPLATES_H
#define DATASTRUCTURETEMPLATES_H

#include <GURUX_DLMS/Objects/GXDLMSObject.h>
#include <GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSProfileGeneric.h>
#include <ApplicationConfigParser/Configurations/LoadedUserConfiguration.h>



typedef struct profileGenericWriterStruct {
    CGXDLMSProfileGeneric *profileGeneric;
    CGXDLMSObjectCollection *serverObjects;
    LoadedUserConfiguration *loadedUserConfiguration;
    pthread_t writerThread;
    pthread_mutex_t *objectAccessMutex;

    pthread_mutex_t *endMutex;
    pthread_cond_t *endCondition;
    bool *endFlag;

    int *m_ServerSocket;
} ProfileGenericWriterStruct;




#endif // DATASTRUCTURETEMPLATES_H_
