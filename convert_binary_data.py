import os

def ttf_to_cpp_array(input_file, output_file):
    with open(input_file, 'rb') as file:
        data = file.read()

    with open(output_file, 'w') as header:
        header.write('#ifndef FONT_DATA_HPP\n')
        header.write('#define FONT_DATA_HPP\n\n')
        header.write('const unsigned char font_data[] = {\n')

        for byte in data:
            header.write(f' 0x{byte:02X},')

        header.write('\n};\n\n')
        header.write(f'const unsigned int font_data_size = {len(data)};\n\n')
        header.write('#endif\n')

if __name__ == "__main__":
    input_font_file = "font_counter.ttf"
    output_header_file = "font_data.hpp"

    if os.path.exists(input_font_file):
        ttf_to_cpp_array(input_font_file, output_header_file)
        print(f"Conversion complete. Binary data saved in {output_header_file}")
    else:
        print(f"Error: Font file '{input_font_file}' not found.")