#include <stdio.h>
#include <stdlib.h>
#include "logFile.h"
#include "anomaly_error.h"
void anomaly_log(const char *message, const char *date,const char *time) {

    //if ther is no "error" message it will print NULL
    FILE *logFile = fopen(LOG_ANOMALY_FILE, "a");

    if (logFile != NULL) {
          fprintf(logFile,"[%s %s]: %s",date,time,message);
    }
    else{
        logError("Error opening anomaly file",NULL);
    }
    fclose(logFile);
}
