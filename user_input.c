#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>


char *make_url() {
    // Your base API URL
    const char *baseURL = "https://api.openweathermap.org/data/2.5/weather?q=";

    // Get user input for the city
    char city[100];
    printf("Enter the city name: ");
    fgets(city, sizeof(city), stdin);

    // Remove newline character from user input
    size_t len = strlen(city);
    if (len > 0 && city[len - 1] == '\n') {
        city[len - 1] = '\0';
    }

    // Get user input for the limit
    // int limit;
    // printf("Enter the limit (e.g., 5): ");
    // scanf("%d", &limit);

    // Get user input for the API key
    char apiKey[100] = "38ae9bcfd18b0c93ce389640e87d7e59";

    // Allocate memory for the final API URL
    char *finalURL = (char *)malloc(500);  // Adjust the size based on your needs

    // Check if memory allocation was successful
    if (!finalURL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Construct the final API URL
    snprintf(finalURL, 500, "%s%s&appid=%s", baseURL, city, apiKey);
    // Print the final URL
    // printf("%s",finalURL);
    return finalURL;
}
// int main(){
//    char *url = make_url();
// }