#include <stdio.h>

struct WeatherData {
    double temperature;
    double humidity;
    // ... other data members
};

void processWeatherData(const char *rawDataFile, const char *processedDataFile) {
    // Assuming there's a function to read historical data from the file
    struct WeatherData historicalData[MAX_HISTORY_SIZE];  // Assuming MAX_HISTORY_SIZE is defined
    int numHistoricalData = readHistoricalDataFromFile(rawDataFile, historicalData);

    // ... (existing code)

    // Calculate simple statistics, including historical data
    double sumTemperature = weather.temperature;  // current temperature
    double sumHumidity = weather.humidity;        // current humidity

    for (int i = 0; i < numHistoricalData; i++) {
        sumTemperature += historicalData[i].temperature;
        sumHumidity += historicalData[i].humidity;
    }

    // Calculate averages
    double averageTemperature = sumTemperature / (numHistoricalData + 1);  // +1 for the current data
    double averageHumidity = sumHumidity / (numHistoricalData + 1);

    // Print statistics to the processed data file
    fprintf(processedFile, "\nSimple Statistics:\n");
    fprintf(processedFile, "Average Temperature: %.2f\n", averageTemperature);
    fprintf(processedFile, "Average Humidity: %.2f\n", averageHumidity);

    // ... (existing code)
}

// Example function to read historical data from file
int readHistoricalDataFromFile(const char *filename, struct WeatherData *data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening historical data file.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%lf,%lf", &data[count].temperature, &data[count].humidity) == 2) {
        count++;
    }

    fclose(file);
    return count;
}

