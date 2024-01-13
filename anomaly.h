#ifndef ANOMALY_H
#define ANOMALY_H
int detect_anomaly(float avg_pressure, float avg_humidity, float current_pressure, float current_humidity,const char *date, const char *time);
#endif