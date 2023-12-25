#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
// #include "user_input.c"
//return value of 'size_t' is byte, 'contents' holds data that recieve/arrives,'size' is size/byte taken by each element,'nmemb' is number of elements/member
size_t write_callback(void *contents, size_t size, size_t nmemb, FILE *file) {
    return fwrite(contents, size, nmemb, file);
}

int main(void) {
    CURL *curl;
    CURLcode result;
    FILE *file;
    //'curl' handle of request
    curl = curl_easy_init();
    
    if (curl == NULL) {
        fprintf(stderr, "HTTP request failed\n");
        return -1;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?q=Islamabad&appid=38ae9bcfd18b0c93ce389640e87d7e59");

    // Open a file for writing
    file = fopen("response.txt", "ab");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        curl_easy_cleanup(curl);
        return -1;
    }

    // Set the write callback function to write to the file the data that is received/arrives from request 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    // Perform the HTTP request,getting response from request
    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
    }

    // Clean up and free resources
    fprintf(file,"\n");
    fclose(file);
    curl_easy_cleanup(curl);

    return 0;
}
