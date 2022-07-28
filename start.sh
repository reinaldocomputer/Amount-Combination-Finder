#! /bin/bash
echo Compiling
g++ main.cpp -o bin

echo "Welcome to Amount Cobination Finder"
echo "Input file name (Ex: input.csv): "
read inputFile
echo "Output file name (Ex: output.csv): "
read outputFile
echo "Amount target to be found: "
read targetValue
echo "IRF target to be found: "
read targetIRF
echo "Number of elements for combination: "
read nElements
echo "Algorithm 1 or 2"
read algorithm

if [ $algorithm = 2 ]
then
echo "Running Algorithm 2"
./bin $inputFile $outputFile $targetValue $targetIRF $nElements algorithm2
else
echo "Running Algorithm 1"
./bin $inputFile $outputFile $targetValue $targetIRF $nElements algorithm1
fi

echo "Finished execution file were created."
