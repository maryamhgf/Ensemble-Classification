#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <bits/stdc++.h> 
#include<numeric>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

#define CURRENT_DIRECTORY "./"
#define APPEND_TO_DATA_DIRECTORY "/dataset"
#define APPEND_TO_LABEL_DIRECTORY "/labels"
#define APPEND_TO_CLASSIFIERS_DIRECTORY "/classifier_"
#define DIRECTORY_ERROR "Couldn't open the directory"
#define CSV_DIRECTORY_ERROR "Couldn't open the csv directory"
#define CSV_AFFIX ".csv"
#define SIZE_ERROR "ERROR: Sizes are incompatible!"
#define PERCENT_SIGN "%"
#define CSV_FILE_DELIMITER ','

typedef vector<float> OneDimensionalInformation;
typedef vector<vector<float>> TwoDimensionalInformation;
typedef vector<vector<vector<float>>> ThreeDimensionalInformation;
typedef const char Folder;
typedef vector<int> OneDimensionalClassesInformation;
typedef vector<vector<int>> TwoDimensionalClassesInformation;


string datasetsFilenameMaker(Folder *folderName);
string labelsFilenameMaker(Folder *folderName);
string classifiersDirectoryMaker(Folder *folderName);
int numberOfFiles (Folder *directoryName);
int numberOfLines(string filename);
TwoDimensionalInformation csvFileReader(string filename);
ThreeDimensionalInformation classifiersInf(Folder *classifiersFolderleName);
TwoDimensionalInformation datasetFileReader(Folder *datasFolderleName);
TwoDimensionalInformation labelsFileReader(Folder *labelsFolderleName);
int whichClass(float classZeroScore, float classOneScore, float classTwoScore);
int classOfData(TwoDimensionalInformation classifierInf, OneDimensionalInformation dataInf);
OneDimensionalClassesInformation allClassifiersClassOfData(ThreeDimensionalInformation infOfAllClassifiers, OneDimensionalInformation dataInf);
TwoDimensionalClassesInformation classesOfAllDatas(ThreeDimensionalInformation infOfAllClassifiers, TwoDimensionalInformation datas);
int majorityOfDataClasses(OneDimensionalClassesInformation classes);
OneDimensionalClassesInformation majorityOfAllDataClasses(TwoDimensionalClassesInformation allDatasClasses);
OneDimensionalClassesInformation compareTwoVectors(OneDimensionalClassesInformation vect1, OneDimensionalClassesInformation vect2);
float accuracyComputer(OneDimensionalClassesInformation comparisonResult);
void accuracyPrinter(float accuracy);
void accuracyOfEnsembleClassification(OneDimensionalClassesInformation comparisonResult);
OneDimensionalClassesInformation twoDToOneDConvertor(TwoDimensionalInformation twoDVector);
OneDimensionalClassesInformation compareResults(OneDimensionalClassesInformation resultOfClasses, Folder *labelsFolderleName);
OneDimensionalClassesInformation outputOfEnsembleClassification(Folder *datasFolderleName, Folder *classifiersFolderleName);
void func(Folder *classifiersFolderleName, Folder *datasAndLabelsFolderleName);

int main(int argc , char *argv[]) {
	Folder *classifiersFolderleName = argv[2];
	Folder *datasAndLabelsFolderleName = argv[1];
	OneDimensionalClassesInformation resultOfClasses = outputOfEnsembleClassification(datasAndLabelsFolderleName, classifiersFolderleName);
	OneDimensionalClassesInformation comparisonResult =  compareResults(resultOfClasses, datasAndLabelsFolderleName);
	accuracyOfEnsembleClassification(comparisonResult);
}

string datasetsFilenameMaker(Folder *folderName) {
	string currentFolder = CURRENT_DIRECTORY;
	string dataFolderName = currentFolder.append(folderName);
	string dataFilename = dataFolderName.append(APPEND_TO_DATA_DIRECTORY);
	return dataFilename;
}

string labelsFilenameMaker(Folder *folderName) {
	string currentFolder = CURRENT_DIRECTORY;
	string labeldFolderName = currentFolder.append(folderName);
	string labelsFilename = labeldFolderName.append(APPEND_TO_LABEL_DIRECTORY);
	return labelsFilename;
}

string classifiersDirectoryMaker(Folder *folderName) {
	string currentFolder = CURRENT_DIRECTORY;
	string classifiersFilename = currentFolder.append(folderName);
	return classifiersFilename;	
}

int numberOfFiles (Folder *directoryName) {
	DIR *directory;
	int number = 0;     
	directory = opendir(directoryName);
	if(directory != NULL) {
		while(readdir(directory))
    		number++;
  		(void) closedir(directory);
  	}
  	else
    	perror(DIRECTORY_ERROR);
  	return number - 2;
}

int numberOfLines(string filename) {
	fstream inputFile;
	inputFile.open(filename, ios::in);
	if(inputFile.fail())
		perror(DIRECTORY_ERROR);
	string s;
	int number = 0;
	while(getline(inputFile, s)) {
		number = number + 1;
	} 
	return number - 1;
}

TwoDimensionalInformation csvFileReader(string filename) {
	fstream inputFile;
	string file = filename + CSV_AFFIX;
	int lineNumber = numberOfLines(file);
	inputFile.open(file, ios::in);
	if(inputFile.fail())
		perror(CSV_DIRECTORY_ERROR);
	TwoDimensionalInformation fileInformation(lineNumber);
	string temp, word;
	getline(inputFile, temp);
	int numOfRows = 0;
	while(getline(inputFile, temp)){
		stringstream s(temp);
		while(getline(s, word, CSV_FILE_DELIMITER))
			fileInformation[numOfRows].push_back(atof(word.c_str()));
		numOfRows = numOfRows + 1;
	}
	return fileInformation;
}

ThreeDimensionalInformation classifiersInf(Folder *classifiersFolderleName) {
	string classifiersDirectory = classifiersDirectoryMaker(classifiersFolderleName);
	int numOfClassifers = numberOfFiles(classifiersDirectory.c_str());
	ThreeDimensionalInformation InfOfAllClassifiers;
	string filename;
	string temp_string = classifiersDirectory + APPEND_TO_CLASSIFIERS_DIRECTORY;
	TwoDimensionalInformation InfOfClassifier;
	for(int i = 0; i<numOfClassifers; i++) {
		string filename = temp_string + to_string(i);
		InfOfClassifier = csvFileReader(filename);
		InfOfAllClassifiers.push_back(InfOfClassifier);
	}
	return InfOfAllClassifiers;
}

TwoDimensionalInformation datasetFileReader(Folder *datasFolderleName) {
	string datasFileName = datasetsFilenameMaker(datasFolderleName);
	TwoDimensionalInformation datasets = csvFileReader(datasFileName);
	return datasets;
}

TwoDimensionalInformation labelsFileReader(Folder *labelsFolderleName) {
	string labelsFileName = labelsFilenameMaker(labelsFolderleName);
	TwoDimensionalInformation labels = csvFileReader(labelsFileName);
	return labels;
}


int whichClass(float classZeroScore, float classOneScore, float classTwoScore) {
	if(classZeroScore >= classOneScore && classZeroScore >= classTwoScore)
		return 0;
	if(classOneScore >= classZeroScore && classOneScore >= classTwoScore)
		return 1;
	if(classTwoScore >= classZeroScore && classTwoScore >= classOneScore)
		return 2;
}

int classOfData(TwoDimensionalInformation classifierInf, OneDimensionalInformation dataInf) {
	float classZeroScore = classifierInf[0][0] * dataInf[0] + classifierInf[0][1] * dataInf[1] + classifierInf[0][2];
	float classOneScore = classifierInf[1][0] * dataInf[0] + classifierInf[1][1] * dataInf[1] + classifierInf[1][2];
	float classTwoScore = classifierInf[2][0] * dataInf[0] + classifierInf[2][1] * dataInf[1] + classifierInf[2][2];
	int result = whichClass(classZeroScore, classOneScore, classTwoScore);
	return result;
}

OneDimensionalClassesInformation allClassifiersClassOfData(ThreeDimensionalInformation infOfAllClassifiers, OneDimensionalInformation dataInf) {
	OneDimensionalClassesInformation classes;
	int dataClass;
	for(int i = 0; i < infOfAllClassifiers.size(); i++) {
		dataClass = classOfData(infOfAllClassifiers[i], dataInf);
		classes.push_back(dataClass);
	}
	return classes;
}

TwoDimensionalClassesInformation classesOfAllDatas(ThreeDimensionalInformation infOfAllClassifiers, TwoDimensionalInformation datas) {
	OneDimensionalClassesInformation dataClass;
	TwoDimensionalClassesInformation allDatasClasses; 
	for(int i = 0; i < datas.size(); i++) {
		dataClass = allClassifiersClassOfData(infOfAllClassifiers, datas[i]);
		allDatasClasses.push_back(dataClass);
	}
	return allDatasClasses;
}

int majorityOfDataClasses(OneDimensionalClassesInformation classes) {
	int numOfClassZero = 0;
	int numOfClassOne = 0;
	int numOfClassTwo = 0;
	for(int i= 0; i < classes.size(); i++){
		if(classes[i] == 0)
			numOfClassZero = numOfClassZero + 1;
		if(classes[i] == 1)
			numOfClassOne = numOfClassOne + 1;
		if(classes[i] == 2)
			numOfClassTwo = numOfClassTwo + 1;
	}
	int result = whichClass(numOfClassZero, numOfClassOne, numOfClassTwo);
	return result;
}

OneDimensionalClassesInformation majorityOfAllDataClasses(TwoDimensionalClassesInformation allDatasClasses) {
	OneDimensionalClassesInformation classResult;
	int result;
	for(int i = 0; i < allDatasClasses.size(); i++){
		result = majorityOfDataClasses(allDatasClasses[i]);
		classResult.push_back(result);
	}
	return classResult;
}

OneDimensionalClassesInformation compareTwoVectors(OneDimensionalClassesInformation vector1, OneDimensionalClassesInformation vector2) {
	vector<int> result;
	if(vector1.size() != vector2.size())
		cout<<SIZE_ERROR<<endl;
	for(int i=0; i < vector1.size(); i++) {
		if(vector1[i] == vector2[i])
			result.push_back(1);
		else
			result.push_back(0);
	}
	return result;
}

float accuracyComputer(OneDimensionalClassesInformation comparisonResult) {
	float sumOfResults = accumulate(comparisonResult.begin(), comparisonResult.end(), 0);
	float sizeOfResult = comparisonResult.size();
	float percent = 100;
	float accuracy = (sumOfResults * percent)/sizeOfResult;
	return accuracy;
}

void accuracyPrinter(float accuracy) {
	cout <<fixed;
    cout << setprecision(2);
	cout<<"Accuracy: "<<accuracy<<PERCENT_SIGN<<endl;
}

void accuracyOfEnsembleClassification(vector<int> comparisonResult) {
	float accuracy = accuracyComputer(comparisonResult);
	accuracyPrinter(accuracy);
}

OneDimensionalClassesInformation twoDToOneDConvertor(TwoDimensionalInformation twoDVector) {
	OneDimensionalClassesInformation oneDVector;
	for(int i=0; i < twoDVector.size(); i++){
		oneDVector.push_back(twoDVector[i][0]);
	}
	return oneDVector;
}

OneDimensionalClassesInformation compareResults(OneDimensionalClassesInformation resultOfClasses, Folder *labelsFolderleName) {
	TwoDimensionalInformation labels = labelsFileReader(labelsFolderleName);
	OneDimensionalClassesInformation oneDLabels = twoDToOneDConvertor(labels);
	OneDimensionalClassesInformation comparisonResult = compareTwoVectors(oneDLabels, resultOfClasses);
	return comparisonResult;
}

OneDimensionalClassesInformation outputOfEnsembleClassification(Folder *datasFolderleName, Folder *classifiersFolderleName) {
	ThreeDimensionalInformation informationOfAllClassifiers = classifiersInf(classifiersFolderleName);
	TwoDimensionalInformation dataSets;
	dataSets = datasetFileReader(datasFolderleName);
	TwoDimensionalClassesInformation allDataClasses;
	allDataClasses = classesOfAllDatas(informationOfAllClassifiers, dataSets);
	OneDimensionalClassesInformation resultOfClasses;
	resultOfClasses = majorityOfAllDataClasses(allDataClasses);
	return resultOfClasses;
}