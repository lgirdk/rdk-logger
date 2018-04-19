/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include "libIBus.h"
#include "dynamicLogger.h"
#include "rdk_log_reg.h"

#define LENGTH_1 64
#define LENGTH_2 64
#define LENGTH_NULL 4

typedef struct rdk_logger_component_details{
    char appName[LENGTH_1];
    char compName[LENGTH_1];
    char subCompName[LENGTH_1];
    char logLevel[LENGTH_2];
    int logStatus;
    rdk_logger_logCtrlCallback_t LOGCTRL_CB;
}rdk_logger_component_details_t;

static GList *registeredCompList = NULL;
static char appName[LENGTH_1] = "";
static int initStatus = 0;
static rdk_logger_logCtrlCallback_t defaultLogger_CB = NULL;

/**
 * The displayGList function will display the details of the registered components.
 * Used only for debugging purpose.
 *
 * @param none
 * @return none
 *
 */
void displayGList()
{
    GList *list = NULL;
    rdk_logger_component_details_t *compDetails = NULL;

    registeredCompList = g_list_first(registeredCompList);
    printf(" ************Displaying GList for %s *************\n",appName);
    for(list = registeredCompList; list != NULL; list = list->next)
    {
        /*Traverse through each member of the list and display the details*/
        compDetails = (rdk_logger_component_details_t *)list->data;
        if(NULL != compDetails)
        {
            printf(" COMPONENT NAME : %s , SUB-COMPONENT NAME : %s \n",
                     compDetails->compName, compDetails->subCompName);
        }
    }
    fflush(stdout);
    return;
}

/**
 * The EvtHandler function will be registered for receiving IARM event
 * from rdklogctrl utility.
 *
 * @param owner Expecting dynamic logger name
 * @param eventId Expecting dynamic logger event log status
 * @param data IARM event data
 * @param len Event data length
 * @return none
 *
 */

void EvtHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
{
    IARM_Bus_DynamicLogger_EventData_t *eventData = NULL;
    rdk_logger_component_details_t *compDetails = NULL;
    GList *comp_list = NULL;
    int cbFound = 0;

    if((NULL == owner) || (NULL == data))
    {
        return;
    }

    eventData = (IARM_Bus_DynamicLogger_EventData_t *)data;

    //printf("Event Details : owner(Expecting %s) : %s, eventId(Expecting %d) : %d, Event App Name(Expecting %s) : %s !!! \n", IARM_BUS_DYNAMIC_LOGGER_NAME, owner, IARM_BUS_DYNAMICLOGGER_EVENT_LOG_STATUS, eventId, appName, eventData->appName);

    /*Confirm whether the intended event is received*/
    if((0 != strncmp(owner, IARM_BUS_DYNAMIC_LOGGER_NAME, strlen(IARM_BUS_DYNAMIC_LOGGER_NAME))) ||
       (IARM_BUS_DYNAMICLOGGER_EVENT_LOG_STATUS != eventId) ||
       (0 != strncmp(appName, eventData->appName, LENGTH_1)))
    {
        printf("App Name comparison failed!!! \n");
        return;
    }

    comp_list = g_list_first(registeredCompList);

    if(comp_list != NULL) {
        do
        {
            //printf("Trying to find out the component!!! \n");
            /*Traverse through the list to identify the component*/
            compDetails = (rdk_logger_component_details_t *) comp_list->data;
            if(NULL != compDetails)
            {
                //printf("Event Details : ModuleName(Expecting %s) : %s, SubModuleName(Expecting %s) : %s \n", compDetails->compName,eventData->moduleName, compDetails->subCompName, eventData->subModuleName);
                if((0 == strncmp(compDetails->compName, eventData->moduleName,LENGTH_1)) &&
                   (0 == strncmp(compDetails->subCompName, eventData->subModuleName, LENGTH_1)))
                {
                    printf("Sending Event to %s \n", eventData->moduleName);
                    compDetails->LOGCTRL_CB(eventData->moduleName, eventData->subModuleName, eventData->logLevel,eventData->log_status);
                    cbFound = 1;
                    break;
                }
            }
        }while((comp_list = g_list_next(comp_list)) != NULL);

        if (0 == cbFound)
            printf("Could not find requested Module/SubModule \n");
    }
    if((NULL != defaultLogger_CB) && (0 == cbFound))
    {
        defaultLogger_CB(eventData->moduleName, eventData->subModuleName, eventData->logLevel,eventData->log_status);
        printf("Default Logger CB called \n");
    }
}

/**
 * The rdk_logger_initialize function will initialize the log registration utility.
 *
 * @param none
 * @return none
 *
 */
void rdk_logger_initialize()
{
    if(0 != initStatus)
    {
        printf("Dynamic logger already initialized \n");
        return;
    }
    initStatus = 1;
    memset(appName,0,sizeof(appName));

    /*Register for dynamic logger event*/
    IARM_Bus_RegisterEventHandler(IARM_BUS_DYNAMIC_LOGGER_NAME, IARM_BUS_DYNAMICLOGGER_EVENT_LOG_STATUS, EvtHandler);
    return;
}

/**
 * The rdk_logger_unInitialize function will unInitialize the log registration utility.
 *
 * @param none
 * @return none
 *
 */
void rdk_logger_unInitialize()
{
    if(1 != initStatus)
    {
        printf("Dynamic logger not initialized \n");
        return;
    }
    GList *list = NULL;
    rdk_logger_component_details_t *compDetails = NULL;

    /*Un-Register for dynamic logger event*/
    IARM_Bus_UnRegisterEventHandler(IARM_BUS_DYNAMIC_LOGGER_NAME, IARM_BUS_DYNAMICLOGGER_EVENT_LOG_STATUS);

    /*Clean up the memory allocated for the glist*/
    registeredCompList = g_list_first(registeredCompList);
    for(list = registeredCompList; list != NULL; list = list->next)
    {
        compDetails = (rdk_logger_component_details_t *)list->data;
        if(NULL != compDetails)
        {
            free(compDetails);
            compDetails = NULL;
        }
    }
    if(NULL != registeredCompList)
    {
        g_list_free(registeredCompList);
        registeredCompList = NULL;
    }
    memset(appName,0,sizeof(appName));
    initStatus = 0;

    return;
}

/**
 * The rdk_logger_setAppName function can be used to
 * register the Application name which intends to use the
 * dynamic logging feature for its components.Upon initialization,
 * each application should register its unique name.
 *
 * @param appName Application name.
 * The appName should try to match the executable name of the application.
 * E.g. "Receiver", "CECDevMgr"
 *
 * @return none
 *
 */
void rdk_logger_setAppName(const char * App)
{
    if(1 != initStatus)
    {
        printf("Dynamic logger not initialized \n");
        return;
    }
    if(App != NULL)
    {
        strncpy(appName ,App, (sizeof(appName) - 1));
    }
    else
    {
        printf("App already registered!!");
    }
    return;
}

/**
 * The rdk_logger_isComponentPresent is an internal function which is used to compare
 * each glist entry.
 *
 * @param pa Parameter 1 for comparison
 * @param pb Parameter 2 for comparison
 * @return none
 *
 */
static gint rdk_logger_isComponentPresent(gconstpointer pa, gconstpointer pb)
{
    const rdk_logger_component_details_t *a = (rdk_logger_component_details_t *)pa;
    const rdk_logger_component_details_t *b = (rdk_logger_component_details_t *)pb;

    return (strncmp(a->compName, b->compName, LENGTH_1) || strncmp(a->subCompName, b->subCompName, LENGTH_1));

}

/**
 * The rdk_logger_registerLogCtrlComp function can be used to 
 * register the component and sub-component name.
 *
 * @param componentName Name of the component
 * @param subComponentName Name of the sub component
 * @param CB is function pointer which is registered as callback function
 * from the component.
 *
 * @return none
 *
 */
void rdk_logger_registerLogCtrlComp(const char* module,const char* subModule,rdk_logger_logCtrlCallback_t CB)
{
    if(1 != initStatus)
    {
        printf("Dynamic logger not initialized \n");
        return;
    }
    rdk_logger_component_details_t *modDetails = NULL;
    GList * moduleNode = NULL;

    if(NULL == CB)
    {
        return;
    }
    if(appName[0] == '\0')
    {
        printf("App not registered!!");
        return;
    }
    if(NULL == module)
    {
        defaultLogger_CB = CB;
        return;
    }

    if(NULL == modDetails)
    {
        modDetails = (rdk_logger_component_details_t *) malloc(sizeof(rdk_logger_component_details_t));
        memset(modDetails, 0, sizeof(rdk_logger_component_details_t));

        if(NULL != modDetails)
        {
            strncpy(modDetails->appName,appName,(sizeof(modDetails->appName) - 1));
            strncpy(modDetails->compName,module,(sizeof(modDetails->compName) - 1));
            /*If sub module details are not provided during registration, copy 'NULL' to the list*/
            if(NULL != subModule)
            {
                strncpy(modDetails->subCompName,subModule,(sizeof(modDetails->subCompName) - 1));
            }
            else
            {
                strncpy(modDetails->subCompName,"NULL",(sizeof(modDetails->subCompName) - 1));
            }
            modDetails->LOGCTRL_CB = CB;
        }
    }

    moduleNode = g_list_find_custom(registeredCompList, modDetails,rdk_logger_isComponentPresent);
    if(NULL != moduleNode)
    {
        printf("Dynamic Log Reg: Component Already registered\n");
        free(modDetails);
        modDetails = NULL;
    }
    else
    {
        registeredCompList = g_list_append(registeredCompList, modDetails);
    }
    //displayGList();
    return;
}
/**
 * The rdk_logger_unRegisterLogCtrlComp function can be used to 
 * unRegister the component and/or sub-component name.
 *
 * @param componentName Name of the component
 * @param subComponentName Name of the sub component
 *
 * @return none
 *
 */
void rdk_logger_unRegisterLogCtrlComp(const char* module,const char* subModule)
{
    if(1 != initStatus)
    {
        printf("Dynamic logger not initialized \n");
        return;
    }
    char lModule[LENGTH_1] = "";
    char lsubModule[LENGTH_1] = "";
    rdk_logger_component_details_t *compDetails = NULL;
    GList *comp_list = NULL;

    if(NULL == module)
    return;

    strncpy(lModule, module, (sizeof(lModule) - 1));

    if(NULL == subModule)
        strncpy(lsubModule, "NULL", (sizeof(lsubModule) - 1));
    else
        strncpy(lsubModule, subModule, (sizeof(lsubModule) - 1));
    /*Traverse through the glist and remove the unregistered component*/
    comp_list = g_list_first(registeredCompList);

    if(comp_list != NULL) {
        do
        {
            compDetails = (rdk_logger_component_details_t *) comp_list->data;
            if(NULL != compDetails)
            {
                if((0 == strncmp(compDetails->compName, lModule, LENGTH_1)) &&
                   (0 == strncmp(compDetails->subCompName, lsubModule, LENGTH_1)))
                {
                    registeredCompList = g_list_remove(registeredCompList,(void *)compDetails);
                    free(compDetails);
                    break;
                }
            }
        }while((comp_list = g_list_next(comp_list)) != NULL);
    }
    //displayGList();
    return;
}
