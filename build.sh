#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status.

# Function to minify HTML file
minify_html() {
    echo "Minifying HTML..."
    html-minifier-terser --remove-attribute-quotes \
                         --remove-comments \
                         --collapse-whitespace \
                         --case-sensitive \
                         --collapse-boolean-attributes \
                         --remove-optional-tags \
                         --remove-redundant-attributes \
                         --conservative-collapse \
                         --process-conditional-comments \
                         --minify-css '{ "level": { "1": { "all": true } } }' \
                         --minify-js '{ "mangle": true , "compress": { "passes": 2 } }' \
                         "index.html" -o "index.min.html" \
                        # --minify-js '{ "compress": { "hoist_funs": true, "hoist_vars": true, "keep_fargs": false, "keep_fnames": false, "passes": 2 } }'\
                        #   --remove-empty-elements \
                        #   --continue-on-parse-error \
                        
}

# Function to compress HTML file using GZIP
gzip_html() {
    echo "Compressing HTML..."
    gzip -9 -c "index.min.html" > "index.min.html.gz"
}

# Function to convert compressed HTML to C array
html_to_c_array() {
    echo "Converting to C array format..."
    # Embed the GZipped content directly into the C file
    echo "" > httptext.h
    xxd -i -n homeHTML "index.min.html.gz" "httptext.h"
}

# Main Build Process
build() {
    echo "Starting build process..."

    # Step 1: Minify the HTML
    minify_html

    # Step 2: Compress the minified HTML file
    gzip_html

    # Step 3: Generate the C array and update httptext.h
    html_to_c_array

    # Clean up temporary files
    rm -f index.min.html index.min.html.gz

    echo "Build process completed successfully!"
}

# Call the main build function
build
