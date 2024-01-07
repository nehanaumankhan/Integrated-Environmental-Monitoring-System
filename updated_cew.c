#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON/cJSON.h"

size_t write_callback(void *contents, size_t size, size_t nmemb, char **output) {
    size_t realsize = size * nmemb;

    *output = realloc(*output, realsize + 1);
    if (*output == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return 0;
    }

    memcpy(*output, contents, realsize);
    (*output)[realsize] = '\0';

    return realsize;
}
//Defining a struct for weather data
struct WeatherData {
    double humidity;
    double pressure;
};

// Function to process temperature, humidity, and vapor pressure from raw data

void processWeatherData(const char *response, const char *processedDataFile) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    cJSON *root = cJSON_Parse(response);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d,%H:%M:%S", tm_info);

    // Check for parsing errors
    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        // free(response);
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
        // free(response);
        return;
    }
    
        // Open the processed data file for writing
    FILE *processedFile = fopen(processedDataFile, "a");

    if (!processedFile) {
        fprintf(stderr, "Error opening the processed data file\n");
        cJSON_Delete(root);
        // free(response);
        return;
    }
  
    fprintf(processedFile,"%f, %f,%s\n",weather.humidity,weather.pressure,buffer);

    // Close the processed data file
    fclose(processedFile);

    // Clean up allocated memory
    cJSON_Delete(root);

    //Storing average values in "average.csv" file

    FILE *avg=fopen("average.csv","a+");
    fseek(avg, 0, SEEK_END);  // Move the file pointer to the end of the file
    if (ftell(avg) == 0) {
    fprintf(avg,"%f, %f,%d\n",weather.humidity,weather.pressure,1);
    }
    else{
       fseek(avg, 0, SEEK_SET);
       float humidity,pressure;
       int days;
       fscanf(avg,"%f,%f,%d",&humidity,&pressure,&days);
       fclose(avg);
       float avg_humidity=((humidity*days)+weather.humidity)/(days+1);
       float avg_pressure=((pressure*days)+weather.pressure)/(days+1);
       days++;
       FILE *avg_ptr=fopen("average.csv","w");
       fprintf(avg_ptr,"%f,%f,%d",avg_humidity,avg_pressure,days);
       fclose(avg_ptr);
    }
}

//main function

int main(void) {

    CURL *curl;
    CURLcode result;
    FILE *file;
    char *response=NULL;
    
    curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "HTTP request failed\n");
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?q=Karachi&appid=38ae9bcfd18b0c93ce389640e87d7e59");

    // Open a file for writing
    file = fopen("response.txt", "ab");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        curl_easy_cleanup(curl);
        return -1;
    }

    // Set the write callback function to write to the file
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    // Perform the HTTP request
    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
    }
    fprintf(file,"%s",response);    

    // Clean up and free resources
    fprintf(file,"\n");
    fclose(file);
    // processWeatherData(response, "processed_data.txt");
    processWeatherData(response, "process.csv");

    curl_easy_cleanup(curl);


    return 0;
}
