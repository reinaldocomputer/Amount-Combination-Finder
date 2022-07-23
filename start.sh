#! /bin/bash
echo Compiling
g++ main.cpp -o bin

echo "Welcome to Amount Cobination Finder"
echo "Input file name (Ex: input.csv): "
read inputFile
echo "Output file name (Ex: output.csv): "
read outputFile
echo "Amount target to be found: "
read keyValue
echo "Number of elements for combination: "
read nElements

echo "Running..."
./bin $inputFile $outputFile $keyValue $nElements > $outputFile
echo "Finished execution file were created."
