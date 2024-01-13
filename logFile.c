#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logFile.h"

void logError(const char *message, const char *error) {

    //if ther is no "error" message it will print NULL
    FILE *logFile = fopen(LOG_FILE, "a");

    if (logFile != NULL) {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);

        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(logFile, "[%s] %s: %s\n", timestamp, message, error);
        fclose(logFile);
    }
}