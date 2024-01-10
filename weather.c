// weather.c
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON/cJSON.h"
#include "weather.h"

// Callback function for handling CURL write operations
size_t write_callback(void *contents, size_t size, size_t nmemb, char **output) {
    size_t realsize = size * nmemb;

    // Allocate memory for output and check for errors
    *output = realloc(*output, realsize + 1);
    if (*output == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return 0;
    }

    // Copy contents to output buffer and null-terminate
    memcpy(*output, contents, realsize);
    (*output)[realsize] = '\0';

    return realsize;
}

// Function to process weather data from the API response
void processWeatherData(const char *response, const char *processedDataFile) {
    // Get current time
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // Parse JSON response
    cJSON *root = cJSON_Parse(response);

    // Buffer to store formatted timestamp
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d,%H:%M:%S", tm_info);

    // Check for parsing errors
    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    // Create a WeatherData structure
    struct WeatherData weather;

    // Access temperature, humidity, and vapor pressure information from the JSON
    cJSON *mainJson = cJSON_GetObjectItem(root, "main");
    cJSON *humidityJson = cJSON_GetObjectItem(mainJson, "humidity");
    cJSON *pressureJson = cJSON_GetObjectItem(mainJson, "pressure");

    // Check if the keys exist and are of the correct type
    if (cJSON_IsNumber(humidityJson) && cJSON_IsNumber(pressureJson)) {
        weather.humidity = humidityJson->valuedouble;
        weather.pressure = pressureJson->valuedouble;
    } else {
        fprintf(stderr, "Invalid or missing data in JSON\n");
        cJSON_Delete(root);
        return;
    }

    // Open the processed data file for writing
    FILE *processedFile = fopen(processedDataFile, "a");

    //check for error occur during opening file
    if (!processedFile) {
        fprintf(stderr, "Error opening the processed data file\n");
        cJSON_Delete(root);
        return;
    }

    // Write humidity, pressure, and timestamp to the processed data file
    fprintf(processedFile, "%f, %f, %s\n", weather.humidity, weather.pressure, buffer);

    // Close the processed data file
    fclose(processedFile);

    // Storing average values in "average.csv" file

    FILE *avg = fopen("average.csv", "a+");
    fseek(avg, 0, SEEK_END); // Move the file pointer to the end of the file

    // If the file is empty, initialize it with the current values and days
    if (ftell(avg) == 0) {
        fprintf(avg, "%f, %f, %d\n", weather.humidity, weather.pressure, 1);
    } 
    // else read humidity, pressure and days from file   
    else {
        fseek(avg, 0, SEEK_SET);
        float humidity, pressure;
        int days;        
        fscanf(avg, "%f,%f,%d", &humidity, &pressure, &days);
        fclose(avg);

        // Calculate new average values and update the days
        float avg_humidity = ((humidity * days) + weather.humidity) / (days + 1);
        float avg_pressure = ((pressure * days) + weather.pressure) / (days + 1);
        days++;

        // Write the updated values to "average.csv" file
        FILE *avg_ptr = fopen("average.csv", "w");
        fprintf(avg_ptr, "%f, %f, %d", avg_humidity, avg_pressure, days);
        fclose(avg_ptr);
    }

    // Clean up allocated memory
    cJSON_Delete(root);
}

// Function to fetch data from the API and process it
int fetchDataAndProcess(void) {
    CURL *curl;
    CURLcode result;
    FILE *file;
    char *response = NULL;

    // Initialize CURL
    curl = curl_easy_init();
    
    //check for any error occuring during initializing curl handle
    if (curl == NULL) {
        fprintf(stderr, "HTTP request failed\n");
        return -1;
    }

    // Set the API URL
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?q=Karachi&appid=38ae9bcfd18b0c93ce389640e87d7e59");

    // Open a file for writing the raw response
    file = fopen("response.txt", "ab");

    //check for errors in opening file
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        curl_easy_cleanup(curl);
        return -1;
    }

    // Set the write callback function to write/store in to the response char
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the HTTP request
    result = curl_easy_perform(curl);

    //handle errors
    if (result != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
    }

    // Write the raw response char(response in json format)  to the txt file
    fprintf(file, "%s", response);
    fprintf(file, "\n");

    // Clean up and free resources
    fclose(file);

    // Process the retrieved data( json response store in response)
    processWeatherData(response, "process.csv");

    // Clean up and free resources
    curl_easy_cleanup(curl);

    return 0;
}
