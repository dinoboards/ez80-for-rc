#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_hex_file> <output_c_file>"
    exit 1
fi

input_file=$1
output_file=$2

# Start the output file with a header guard
echo "#ifndef FIRMWARE_VERSION_H" > $output_file
echo "#define FIRMWARE_VERSION_H" >> $output_file
echo "" >> $output_file

# Read the input file line by line
while IFS=$' \t\r\n' read -r line
do
   # Skip blank lines
    if [[ -z "$line" ]]; then
        continue
    fi

    # Extract the name and value parts
    name=$(echo "$line" | sed 's/^\s*\(.*\)\s\+\.EQU\s\+\(.*\)$/\1/')
    value=$(echo "$line" | sed 's/^\s*\(.*\)\s\+\.EQU\s\+\(.*\)$/\2/')


    # Trim trailing whitespace from name
    name=$(echo "$name" | sed 's/[[:space:]]*$//')
    value=$(echo "$value" | sed 's/[[:space:]]*$//')

    # Check if the line contains DATE
    if [[ "$name" == "DATE" ]]; then
        # Format the DATE line with double quotes
        formatted_line="#define FIRMWARE_DATE \"$value\""
    else
        # Format the line correctly
        formatted_line="#define $name $value"
    fi

    echo "$formatted_line" >> $output_file
done < "$input_file"

# End the header guard
echo "" >> $output_file
echo "#endif // FIRMWARE_VERSION_H" >> $output_file

echo "Conversion complete: $output_file"
