#!/bin/bash

# Step 1: Minify the HTML file using html-minifier-terser
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

# Step 2: Remove the second line from httptext.h and replace it with the content of index.min.html
# {
#   head -n 1 httptext.h    # Keep the first line
#   cat index.min.html      # Insert the contents of index.min.html
#   echo ""
#   tail -n +3 httptext.h   # Append lines starting from the third line
# } > temp.txt
echo "String homeHTML = R\"HTML_asdlkSJLJfl(" > temp.txt
cat index.min.html >> temp.txt
echo "" >> temp.txt
echo "   )HTML_asdlkSJLJfl\";" >> temp.txt

# Step 3: Move the updated content back to httptext.h
mv temp.txt httptext.h

# Step 4: Clean up any temporary files (if applicable)
rm -f temp.txt

echo "Build process completed successfully!"
