/*
 * ============================================================================
 * COMCAST C O N F I D E N T I A L AND PROPRIETARY
 * ============================================================================
 * This file (and its contents) are the intellectual property of Comcast.  It may
 * not be used, copied, distributed or otherwise  disclosed in whole or in part
 * without the express written permission of Comcast.
 * ============================================================================
 * Copyright (c) 2013 Comcast. All rights reserved.
 * ============================================================================
 */

#if !defined(_RDK_LOG_REG_H)
#define _RDK_LOG_REG_H

#ifdef __cplusplus
extern "C"
{
#endif

/*Callback function type which need to be registered by the components to get
log level and log status set dynamically by the user.*/

typedef void (*rdk_logger_logCtrlCallback_t) (const char *moduleName, const char *subComponentName, const char *loggingLevel, int log_status);

/**
 * The rdk_logger_initialize function will initialize the log registration utility.
 *
 * @param none
 * @return none
 *
 */
void rdk_logger_initialize();

/**
 * The rdk_logger_unInitialize function will unInitialize the log registration utility.
 *
 * @param none
 * @return none
 *
 */
void rdk_logger_unInitialize();

/**
 * The rdk_logger_setAppName function can be used to
 * register the Application name which is intends to use the
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
void rdk_logger_setAppName(const char *appName);

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
void rdk_logger_registerLogCtrlComp (const char *componentName, const char *subComponentName, rdk_logger_logCtrlCallback_t CB);

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
void rdk_logger_unRegisterLogCtrlComp(const char *module, const char *subModule);

/**
 * The rdk_logger_registerLogCtrlCallback function can be used to
 * register the callback function from different components.
 *
 * @param cb is function pointer which is registered as callback function
 * from the component.
 *
 * @return none
 *
 */
//void rdk_logger_registerLogCtrlCallback (rdk_logger_logCtrlCallback_t cb);



#ifdef __cplusplus
}
#endif

#endif /* _RDK_LOG_REG_H */

