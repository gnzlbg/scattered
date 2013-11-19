#!/bin/bash 
# Removes all cmake-generated files
# Note: it doesn't remove any binaries
echo "Cleaning up..."

TO_REMOVE="Doxyfile CMakeCache.txt CMakeFiles CTestTestfile.cmake Makefile cmake_install.cmake"

for FILE in $TO_REMOVE
do
    echo "  removing ${FILE} ..."
    find . -name $FILE -print0 | xargs -0 rm -rf
done

# Clean Catch
echo "  removing ext/catch/catch-prefix..."
rm -rf ext/catch/catch-prefix
rm -rf Testing

echo "... clean-up done!"
