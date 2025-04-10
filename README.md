# CSS-Parser

CSS-Parser is a C++ application that parses CSS files and extracts relevant information. It is designed to analyze and manipulate the structure and content of stylesheets, making it easier to process and use CSS data programmatically.

## Features
- Parses CSS files and extracts information about CSS rules.
- Supports various CSS properties and selectors.
- Allows for easy extraction and manipulation of styles.

## Requirements
- C++ Compiler (C++11 or later)
- Standard C++ Library

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/gum3k/CSS-Parser.git
   ```

2. Compile the program using a C++ compiler:
   ```bash
   g++ -o css_parser CSS.cpp
   ```

3. Run the program:
   ```bash
   ./css_parser
   ```

## Usage

To use the parser, simply provide the path to a CSS file. The program will output the relevant parsed information, such as CSS rules, selectors, and properties.

```bash
./css_parser path_to_file.css
```
