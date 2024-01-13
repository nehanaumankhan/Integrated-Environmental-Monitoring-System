#!/bin/bash

# File paths
csv_file="process.csv"
report_pdf="weather_report.pdf"

# Extract humidity and pressure columns, skip header
humidity_column=$(awk -F, 'NR>1 {print $1}' "$csv_file")
pressure_column=$(awk -F, 'NR>1 {print $2}' "$csv_file")
city="Karachi"

# Function to calculate average, minimum, and maximum
calculate_stats() {
    local column_values=$1
    local avg=$(echo "$column_values" | awk '{ total += $1 } END { printf "%.2f", total/NR }')
    local min=$(echo "$column_values" | sort -n | head -n 1)
    local max=$(echo "$column_values" | sort -n | tail -n 1)

    echo "Average: $avg"
    echo "Minimum: $min"
    echo "Maximum: $max"
}

# Generate a plot using gnuplot and save it as a PDF
gnuplot <<- EOF
    set terminal pdf
    set output "$report_pdf"
    plot "-" using 1 with lines title "Humidity", "-" using 2 with lines title "Pressure"
    $humidity_column
    e
    $pressure_column
    e
EOF

# Send email with the PDF report as an attachment
to_email="ayeshaahmed45678@gmail.com"
subject="Weather Report"
body="Please find the attached weather report of Pressure and humidty for $city."
ATTACHMENT_FILE=$report_pdf

# Use the sendemail command to send the email with the PDF attachment
sendemail -f "ayeshaahmed45678@gmail.com" -t "$to_email" -u "$subject" -m "$body" -a "$ATTACHMENT_FILE" -s "smtp.gmail.com:587" -o tls=yes -xu "ayeshaahmed45678@gmail.com" -xp "gdov cqnm egcs rxni"

# Remove the temporary PDF file
rm "$report_pdf"
