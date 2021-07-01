#ifndef __CJSON_PROCESS_H
#define __CJSON_PROCESS_H
#include "cJSON.h"
#include "stdint.h"

#define   NAME            "switch"
#define   LIGHT_STU       "state"

#define   TEMP_NUM        "temp"  
#define   HUM_NUM         "hum" 

#define   DEFAULT_NAME          "switch"    
#define   DEFAULT_STU           1

#define   DEFAULT_TEMP_NUM       25.0 
#define   DEFAULT_HUM_NUM        50.0 


#define   UPDATE_SUCCESS       1 
#define   UPDATE_FAIL          0

extern cJSON *cJSON_Init(void);
extern void Proscess(void *data);
extern uint8_t cJSON_Update(const cJSON * const object,const char * const string,void *d);

#endif