#include "LexiconManager.h"


LexiconManager::LexiconManager() {
	//lexicon = new Lexicon();
	loadWords();
}


LexiconManager::~LexiconManager() {
	//updateLexiconFile();
}

int LexiconManager::findHash(string word) {
	requests++;
//	cout << "Adding new word=" << word << endl;
	unordered_map<string, WPair>::iterator it = lexicon.find(word);
	if(it==lexicon.end()){
		if (lastId <= 2147000000) {
			if (lastId > 2147000000) {
				return -1;
			}

			int hash = lastId++;			
			WPair temp = {hash,1};
			lexicon[word] = temp;
			return hash;
		}
	}
	else {
		it->second.count++;
		return it->second.id;
	}
}

void LexiconManager::updateLexiconFile() {
	//cout << "Updating lexicon file...";
	outputFile.open("D:\\Workspace\\Wikipedia\\Parser\\Lexicon.txt",ios::trunc);
	
	for (hash_map<string, WPair>::iterator it = lexicon.begin(); it != lexicon.end(); it++) {
		outputFile<<(it->first)<<','<<it->second.id<<','<<it->second.count<<endl;
	}
	
	//lexicon->update(*this, "");
	outputFile.close();
}
void LexiconManager::loadWords() {
	cout << "Loading words" << endl;
	ifstream fileIn("D:\\Workspace\\Wikipedia\\Parser\\Lexicon.txt", ios::in);
	int wordId;
	string word;
	int wordCount;
	char temp;
	while (!fileIn.eof()) {
		char c;
		word = "";
		while (true) {
			fileIn >> c;
			if (c == ',')
				break;
			else
				word += c;
		}
		fileIn >> wordId >> temp >> wordCount;
		//cout << "Loading " << wordId << ". " << word << " " << wordCount << endl;
		//lexicon->add(word, wordId, wordCount);
		WPair temp = { wordId, wordCount };
		lexicon[word] = temp;
	}
	lastId = ++wordId;
	cout << "Loaded words n=" << lastId;
	fileIn.close();
}
void LexiconManager::lexiconUpdateHelper(string word, int hash, int count) {
	outputFile << word << "," << hash << "," << count << endl;
}