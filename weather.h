// weather.h
#ifndef WEATHER_H
#define WEATHER_H

#include <stdio.h>
#include <curl/curl.h>
#include "cJSON/cJSON.h"

struct WeatherData {
    double humidity;
    double pressure;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, char **output);
void processWeatherData(const char *response, const char *processedDataFile);
int fetchDataAndProcess(void);

#endif // WEATHER_H
