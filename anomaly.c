#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "anomaly_error.h"
#include "logFile.h"

char buffer[200];
char anomaly[124];

int detect_anomaly(float avg_pressure, float avg_humidity, float current_pressure, float current_humidity,const char *date, const char *time) {
    if ((current_pressure > avg_pressure + 8 || current_pressure < avg_pressure - 8)&&(current_humidity > avg_humidity + 8 || current_humidity < avg_humidity - 8)){
        snprintf(buffer, sizeof(buffer), "Current pressure is %.2f while the average value of pressure in recent days is %.2f\n\nCurrent humidity is %.2f while the average value of humidity in recent days is %.2f\n",current_pressure,avg_pressure,current_humidity,avg_humidity);
        snprintf(anomaly,sizeof(anomaly),"Pressure and humidity are not in range\n ");
    } 
    else if (current_pressure > avg_pressure + 8 || current_pressure < avg_pressure - 8) {
        snprintf(buffer, sizeof(buffer), "Current pressure is %.2f while the average value of pressure in recent days is %.2f\n\n",current_pressure,avg_pressure);
        snprintf(anomaly,sizeof(anomaly),"Pressure is not in range\n ");

    } 
    else if(current_humidity > avg_humidity + 0 || current_humidity < avg_humidity - 8){
        snprintf(buffer,sizeof(buffer) ,"Current humidity is %.2f while the average value of humidity in recent days is %.2f\n",current_humidity,avg_humidity);
        snprintf(anomaly,sizeof(anomaly),"Humidity is not in range\n ");

    }
    
    else{
        return EXIT_SUCCESS; // Return a default value in case of no anomaly
        }
        //generate an  alert in anomaly.log file
        anomaly_log(anomaly,date,time);

        // Replace "mail.sh" with the actual name of your shell script
        char script[] = "/home/ayesha/Desktop/Ayesha/CEW/mail.sh";
        
        // Construct the command with arguments
        char command[256];
        // snprintf(command, sizeof(command), "%s %s %s %s", script,buffer, date,time);
        snprintf(command, sizeof(command), "%s \"%s\" \"%s\" \"%s\"", script, buffer, date, time);

        // Use the system function to execute the shell script with arguments
        int result = system(command);

        // Check the result of the system call
        if (result == -1) {
            perror("Error executing the script");
            logError("Error executing the script",NULL);
            return EXIT_FAILURE;
        } else {
            return EXIT_SUCCESS;
        }
    } 

