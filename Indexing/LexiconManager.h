#pragma once

class LexiconManager;
#include "Lexicon.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class WPair {
public:
	int id;
	int count;
};

class LexiconManager {
	unordered_map<string, WPair>lexicon;
	//Lexicon* lexicon;
	int lastId=1;
	int requests=0;

	ofstream outputFile;
public:
	LexiconManager();
	~LexiconManager();

	int findHash(string word);
	void updateLexiconFile();
	void loadWords();

	void lexiconUpdateHelper(string word, int hash, int count);
};

