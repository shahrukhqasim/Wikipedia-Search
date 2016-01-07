#include "DocumentManager.h"


DocumentManager::DocumentManager() {
	docsFile.open("D:\\Workspace\\Wikipedia\\Parser\\docIds.txt");
}


DocumentManager::~DocumentManager() {
}


int DocumentManager::getDocumentHash(char*document) {
	lastDocId++;
	docsFile << lastDocId<<','<<document<<endl;
	return lastDocId;
}