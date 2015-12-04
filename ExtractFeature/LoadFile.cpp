#include "LoadFile.h"
#include "Configure.h"
#include "Utils.h"

void load(string vocabularyFile, string imgPathProVladFile, string trainFeatureProVladFile,string imgPathDenseSurfFile,string trainFeatureDenseSurfFile) {
	// release vocabulary
	if (vocabulary) freeMatrix(vocabulary, clusterCount);
	// load vocabulary
	vocabulary = newMatrix(clusterCount, surfDescriptorSize);
	loadVocabulary(vocabularyFile, vocabulary, clusterCount, surfDescriptorSize);
	cout << "Load Vocabulary End. " << endl;

	// release trainFeatureMap
	if (!trainFeatureProVladMap.empty()) freeFeatureMap(trainFeatureProVladMap);
	// Load train image feature
	loadFeatureBin(imgPathProVladFile, trainFeatureProVladFile, surfVladFeatureSize, trainFeatureProVladMap, false);
	cout << "Load Train Feature Pro + VLAD  End: " << trainFeatureProVladMap.size() << endl;
	

	// release trainFeatureMap
	if (!trainFeatureDenseSurfMap.empty()) freeFeatureMap(trainFeatureDenseSurfMap);
	// Load train image feature
	loadFeatureBin(imgPathDenseSurfFile, trainFeatureDenseSurfFile, denseSurfFeatureSize, trainFeatureDenseSurfMap, false);
	cout << "Load Train Feature Dense Surf End: "<< trainFeatureDenseSurfMap.size() <<endl;

}