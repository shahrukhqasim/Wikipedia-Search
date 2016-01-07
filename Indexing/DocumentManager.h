#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class DocumentManager {
	long long lastDocId;
	long long lastDocId2;
	ofstream docsFile;
public:
	DocumentManager();
	~DocumentManager();

	int getDocumentHash(char*document);
};

