#!/bin/bash
# Check if at least three arguments are provided
if [ "$#" -lt 3 ]; then
    echo "Usage: $0 arg1 arg2 arg3"
    exit 1
fi

arg1="$1"
arg2="$2"
arg3="$3"

TO="<Email>@gmail.com"
SUBJECT="***Anomaly in weather***"
BODY="An anomaly is detected in the weather data at:\n\nDate: $arg2\nTime: $arg3\n\n        ***ANOMALY***        \n\n$arg1\n\nPlease find the attached report of weather data"
ATTACHMENT_FILE="/home/ayesha/Desktop/Ayesha/CEW/process.csv"

sendemail -f "<Email>@gmail.com" -t "$TO" -u "$SUBJECT" -m "$BODY" -a "$ATTACHMENT_FILE" -s "smtp.gmail.com:587" -o tls=yes -xu "<Email>@gmail.com" -xp "<Password>"
