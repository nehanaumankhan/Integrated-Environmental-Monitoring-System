#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON/cJSON.h"
#include "processData.h"
#include "logFile.h"

#define MAX_LINES 30
#define MAX_LINE_LENGTH 100   //for calculating average
#define MAX_BUFFER_SIZE 1024

struct WeatherData {
    double humidity;
    double pressure;
};

// Function to process temperature, humidity, and vapor pressure from raw data
void processWeatherData(const char *response, const char *processedDataFile, const char *dateBuffer, const char *timeBuffer, float *Humidity, float *Pressure) {
    // Parse the JSON response
    cJSON *root = cJSON_Parse(response);

    // Check for parsing errors
    if (!root) {
        logError("Error parsing JSON", cJSON_GetErrorPtr());
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
        *Humidity = weather.humidity;
        *Pressure = weather.pressure;
    } else {
        logError("Invalid or missing data in JSON", NULL);
        fprintf(stderr, "Invalid or missing data in JSON\n");
        cJSON_Delete(root);
        return;
    }

    // Open the processed data file for writing
    FILE *processedFile = fopen(processedDataFile, "r+");

    if (!processedFile) {
        logError("Error opening the processed data file", NULL);
        fprintf(stderr, "Error opening the processed data file\n");
        cJSON_Delete(root);
        return;
    }

    int entryCount = 0;
    char buffer[MAX_BUFFER_SIZE];

    // Count the number of entries in the processed file
    while (fgets(buffer, sizeof(buffer), processedFile) != NULL) {
        entryCount++;
    }

    // If the entry count exceeds the maximum allowed lines, trim the file
    if (entryCount >= MAX_LINES) {
        FILE *tempFile = fopen("temp.csv", "w");

        if (!tempFile) {
            perror("Error opening temporary file for writing");
            fprintf(stderr, "Error opening temporary file for writing\n");
            fclose(processedFile);
            return;
        }

        fseek(processedFile, 0, SEEK_SET);
        // Read and discard the first and second line in the original file
        fgets(buffer, sizeof(buffer), processedFile);
        fgets(buffer, sizeof(buffer), processedFile);
        // Copy the headers in the temporary file
        fprintf(tempFile, "HUMIDITY(%%),PRESSURE(hPa),DATE,TIME,CITY\n");

        // Copy the remaining entries to the temporary file
        while (fgets(buffer, sizeof(buffer), processedFile) != NULL) {
            fprintf(tempFile, "%s", buffer);
        }

        fclose(tempFile);

        // Close and reopen the processed file after renaming
        fclose(processedFile);
        remove(processedDataFile);
        rename("temp.csv", processedDataFile);
        processedFile = fopen(processedDataFile, "a"); // Open in append mode

        if (!processedFile) {
            perror("Error reopening file for reading and writing");
            exit(EXIT_FAILURE);
        }

        fprintf(processedFile, "%f, %f,%s,%s,%s\n", weather.humidity, weather.pressure, dateBuffer, timeBuffer, "Karachi");
    } else {
        // Append the new response to the file without deleting anything
        fseek(processedFile, 0, SEEK_END);
        fprintf(processedFile, "%f, %f,%s,%s,%s\n", weather.humidity, weather.pressure, dateBuffer, timeBuffer, "Karachi");
    }

    // Clean up and free resources
    fclose(processedFile);

    // Clean up allocated memory
    cJSON_Delete(root);
}

// Function to calculate the average humidity and pressure from processed data
int calculate_average(const char *processedDataFile, const char *averageDataFile, float *avgHumidity, float *avgPressure) {
    FILE *file;
    file = fopen(processedDataFile, "r");
    if (file == NULL) {
        perror("Error opening file");
        logError("Error opening averageDataFile", NULL);
        return 1;
    }

    // Skip the header line
    char header[MAX_LINE_LENGTH];
    fgets(header, sizeof(header), file);

    float totalHumidity = 0.0;
    float totalPressure = 0.0;
    int count = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ",");
        // Assuming temperature is the first column and pressure is the second column
        float humidity = atof(token);
        token = strtok(NULL, ",");
        float pressure = atof(token);

        totalHumidity += humidity;
        totalPressure += pressure;
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No data found in the file.\n");
        return 1;
    }

    *avgHumidity = totalHumidity / count;
    *avgPressure = totalPressure / count;
    file = fopen(averageDataFile, "w");
    fprintf(file, "%.2f,%.2f", *avgHumidity, *avgPressure);
    return 0;
}
