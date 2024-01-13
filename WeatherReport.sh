#!/bin/bash

# Input CSV file
input_file="process.csv"

# Output PNG file
output_file="weather_report.png"
# Gnuplot script file
gnuplot_script="weather_plot_script.gnu"
city="Karachi"

# Generate Gnuplot script
cat <<EOL > $gnuplot_script
set terminal pngcairo enhanced font "arial,10" size 1200,600
set output "$output_file"

set datafile separator ","
set xdata time
set timefmt "%Y-%m-%d,%H:%M:%S"
set format x "%Y-%m-%d\n%H:%M:%S"
set style fill solid 0.3
set key autotitle columnhead
set key noautotitle
set grid x,y
set key right center # legend placement
set xtics rotate by -90
set xlabel "Date and Time"
set ylabel "Humidity (%) / Pressure (hPa)"
set ytics 70
set title "Humidity and Pressure in Karachi"

plot "$input_file" using (timecolumn(3)):2 with linespoints title "Pressure", \
     "$input_file" using (timecolumn(3)):1 with linespoints title "Humidity"
EOL

# Run Gnuplot
gnuplot $gnuplot_script

# Send email with the .png report as an attachment
to_email="ayeshaahmed45678@gmail.com"
subject="Weather Report"
body="Please find the attached weather report of Pressure and humidty for $city."
ATTACHMENT_FILE=$output_file

# Use the sendemail command to send the email with the PDF attachment
sendemail -f "ayeshaahmed45678@gmail.com" -t "$to_email" -u "$subject" -m "$body" -a "$ATTACHMENT_FILE" -s "smtp.gmail.com:587" -o tls=yes -xu "ayeshaahmed45678@gmail.com" -xp "gdov cqnm egcs rxni"

# Remove the temporary PDF file
rm "$output_file"
rm "$gnuplot_script"

echo "Graph generated and saved as $output_file"
