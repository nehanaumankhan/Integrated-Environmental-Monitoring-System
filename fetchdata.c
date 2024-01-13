#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "fetchData.h"
#include "logFile.h"

#define MAX_LINES 30
#define MAX_BUFFER_SIZE 256

// Callback function for writing received data
size_t write_callback(void *contents, size_t size, size_t nmemb, char **output) {
    size_t realsize = size * nmemb;

    *output = realloc(*output, realsize + 1);
    if (*output == NULL) {
        perror("Error opening file");
        logError("Memory allocation error", NULL);
        fprintf(stderr, "Memory allocation error\n");
        return 0;
    }

    memcpy(*output, contents, realsize);
    (*output)[realsize] = '\0';

    return realsize;
}

// Struct to store weather data
struct WeatherData {
    double humidity;
    double pressure;
};

// Function to perform HTTP request and save response to a file
char *perform_http_request(const char *output_filename) {
    CURL *curl;
    CURLcode result;
    FILE *file;
    char *response = NULL;

    // Initialize libcurl
    curl = curl_easy_init();
    if (curl == NULL) {
        // perror("Error opening file");
        logError("Failed to initialize libcurl", NULL);
        fprintf(stderr, "Failed to initialize libcurl\n");
        return NULL;
    }

    // Set the request URL
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?q=Karachi&appid=38ae9bcfd18b0c93ce389640e87d7e59");

    // Open a file for writing
    file = fopen(output_filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        logError("Failed to open file for writing", NULL);
        fprintf(stderr, "Failed to open file for writing\n");
        curl_easy_cleanup(curl);
        return NULL;
    }

    // Set the write callback function to write to the response buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
    }

    // Check and limit the number of lines in the file
    int lineCount = 0;
    char buffer[MAX_BUFFER_SIZE];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;
    }

    if (lineCount >= MAX_LINES) {
        // Rewind to the beginning of the file
        fseek(file, 0, SEEK_SET);

        FILE *tempFile = fopen("temp.txt", "w");
        if (tempFile == NULL) {
            perror("Error opening temporary file for writing");
            exit(EXIT_FAILURE);
        }

        // Skip the first line in the original file
        fseek(file, strlen(fgets(buffer, sizeof(buffer), file)), SEEK_SET);

        // Copy the remaining lines to the temporary file
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            fprintf(tempFile, "%s", buffer);
            // printf("a line %s\n",buffer);
        }

        fclose(tempFile);
        remove("response_copy.txt");
        rename("temp.txt", "response_copy.txt");

        // Truncate the original file
        // freopen("response_copy.txt", "w", file);
        // file = fopen("response_copy.txt", "a");

        // Append the new response to the file
        file = fopen("response_copy.txt", "a");
        if (file == NULL) {
            perror("Error reopening file for reading and writing");
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%s\n", response);
        // printf("Done");
    } else {
        // Append the new response to the file without deleting anything
        fseek(file, 0, SEEK_END);
        fprintf(file, "%s\n", response);
    }

    // Clean up and free resources
    fclose(file);
    curl_easy_cleanup(curl);

    return response;
}
