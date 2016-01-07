#pragma once

#include "Lexicon.h"

#include <iostream>
#include <fstream>

#include "LexiconManager.h"

using namespace std;

class Lexicon;

class DatabaseManager
{
	LexiconManager&lexiconManager;
public:
	DatabaseManager();
	virtual ~DatabaseManager();
	void connect();
	void loadWords(Lexicon& lexicon);
	void updateWord(unsigned int wordId, unsigned int wordCount);
	int addNewDocument(string document);
	int addNewWord(string word);
	void forwardWord(int wordId, int numWord, int docId, string table);
};

