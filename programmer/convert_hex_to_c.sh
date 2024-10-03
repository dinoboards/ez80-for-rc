#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_hex_file> <output_c_file>"
    exit 1
fi

INPUT_HEX_FILE=$1
OUTPUT_C_FILE=$2

# Start the C file with the declaration of the string constant
echo -e '#include <stdint.h>\n' > $OUTPUT_C_FILE
echo 'const uint8_t ez80_rc_firmware[] = {' >> $OUTPUT_C_FILE

# Initialize an empty string to hold the formatted output
formatted_line=""

# Read the hex file line by line and append to the C file
while IFS=$' \t\r\n' read -r line
do
    # Remove the leading colon
    line=${line#:}

    formatted_line+="\t"

    # Loop through the line two characters at a time
    for (( i=0; i<${#line}; i+=2 )); do
        hex_pair=${line:i:2}
        formatted_line+="0x$hex_pair, "
    done

  formatted_line=${formatted_line%?}

  #add a newline
  formatted_line+="\n"

done < "$INPUT_HEX_FILE"

formatted_line=${formatted_line%???}
echo -e "$formatted_line" >> $OUTPUT_C_FILE
echo '};' >> $OUTPUT_C_FILE
echo "Conversion complete: $OUTPUT_C_FILE"
