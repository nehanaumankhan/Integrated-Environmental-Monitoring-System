#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON/cJSON.h"
#include "fetchData.h"
#include "processData.h"
#include "anomaly.h"

#define ReponseFile "response.txt"
#define ProcessedDataFile "process.csv"
#define AverageDataFile  "average.csv"


int main(void) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char dateBuffer[11];  // Buffer for date (YYYY-MM-DD)
    char timeBuffer[9];   // Buffer for time (HH:MM:SS)
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", tm_info);
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", tm_info);

    float avgHumidity, avgPressure,humidity,pressure;

    char *response = perform_http_request(ReponseFile);
   
    processWeatherData(response, ProcessedDataFile,dateBuffer,timeBuffer,&humidity,&pressure);

    calculate_average(ProcessedDataFile,AverageDataFile,&avgHumidity,&avgPressure);

    detect_anomaly(avgPressure,avgHumidity,pressure,humidity,dateBuffer,timeBuffer);
    
    return 0;
}
