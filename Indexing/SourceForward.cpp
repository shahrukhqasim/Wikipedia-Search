#include <iostream>
#include <fstream>
#include <queue>
#include <stdio.h>
#include "Lexicon.h"
#include "DatabaseManager.h"
#include "DocumentManager.h"
#include "sys/stat.h"

using namespace std;

class SRQParser {
	ifstream in;
	bool errorStatus = false;
	queue<char>inputQueue;
	char *currentArticle = new char[4096];
	char word[4096];
	int level = -1;
	DocumentManager&manager;
	LexiconManager&lexicon;
	int docId;

	char *testSpace=new char[1024];

	int numWord;

	ofstream indexFiles[8];
	int zcount = 0;

	char *cache;
	static const int limit = 4194000;
	char* cacheZ[8];
	int cacheIndex[8];
	char*cacheI;


	long long fileLength = 12545834053;


	int inputIndex = 0;
	long long iiIndex = 0;

public:
	~SRQParser() {
	}

	long GetFileSize(std::string filename)
	{
		struct stat stat_buf;
		int rc = stat(filename.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
	}

	SRQParser(string s, DocumentManager&manager, LexiconManager&lexicon) :manager(manager), lexicon(lexicon) {
		cache = new char[33554500];
		cacheI = new char[200000010];
		try {
			in = ifstream(s);
			//5048576000;
			in.read(cacheI, 10485760);
			static const string indexFileNames[8] = { "D:\\Workspace\\Wikipedia\\Parser\\ForwardNormal.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardTitles.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub2.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub3.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub4.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub5.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub6.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub7.txt" };
			for (int i = 0; i < 8; i++) {
				indexFiles[i].open(indexFileNames[i], ios::binary);
				cacheZ[i] = &cache[i * 4194304];
				cacheIndex[i] = 0;
			}

		}
		catch (...) {
			errorStatus = true;
			cout << "Error reading file"<<endl;
		}
	}

	void closeFiles() {
		static const string indexFileNames[8] = { "D:\\Workspace\\Wikipedia\\Parser\\ForwardNormal.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardTitles.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub2.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub3.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub4.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub5.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub6.txt", "D:\\Workspace\\Wikipedia\\Parser\\ForwardSub7.txt" };
		
		for (int i = 0; i < 8; i++) {
			indexFiles[i].write(cacheZ[i], cacheIndex[i]);
			cacheIndex[level] = 0;
		}

		for (int i = 0; i < 8; i++) {
			indexFiles[i].close();
//			indexFiles[i].open(indexFileNames[i],ios::binary);
		}

		lexicon.updateLexiconFile();
	}

	void write(int wordId) {
		const char*x = reinterpret_cast<const char *>(&docId);
		const char*y = reinterpret_cast<const char *>(&wordId);
		for (int i = 0; i < 4; i++) {
			cacheZ[level][cacheIndex[level]++] = x[i];
		}
		for (int i = 0; i < 4; i++) {
			cacheZ[level][cacheIndex[level]++] = y[i];
		}

		if (cacheIndex[level] >= limit) {
			indexFiles[level].write(cacheZ[level], cacheIndex[level]);
			cacheIndex[level] = 0;
		}
	}

	void processWord() {
		if (word[0] == 0)
			return;
		int wordId=lexicon.findHash(word);
		write(wordId);
	}
	void processArticleTitle() {
		numWord = 0;
		docId=manager.getDocumentHash(currentArticle);
		if (docId%1000==0)
			cout << "Got docId=" << docId << " for " << currentArticle << endl;
		queue<char>inputQueue;
		bool flag = false;
		for (int l = 0; l < 1024; l++) {
			char c = currentArticle[l];
			if (flag)
				break;
			if (c == 0) {
				flag = true;
			}
			if (c >= 'A'&&c <= 'Z')
				inputQueue.push(c + 32);
			else if (c >= 'a'&&c <= 'z')
				inputQueue.push(c);
			else {
				int i = 0;
				if (inputQueue.size()>256) {
					while (!inputQueue.empty()) {
						inputQueue.pop();
					}
					continue;
				}
				while (!inputQueue.empty()) {
					word[i++] = inputQueue.front();
					inputQueue.pop();
				}
				word[i] = 0;
				//cout << "Word " << word << " in title " << currentArticle << endl;
				//cin.get();
				processWord();
				numWord++;
			}
		}
	}

	char getChar() {
		if (inputIndex == 200000000) {
			in.read(cacheI, 200000000);
			inputIndex = 0;
		}
		iiIndex++;
		if(iiIndex == fileLength) {
			closeFiles();
			cout << "COMPLETED!!!!" << endl;
			int temp;
			cin >> temp;
			exit(1);
		}
		return cacheI[inputIndex++];
	}

	void execute() {
		if (errorStatus)
			return;
		int articleNumber = 0;
		int lineN= 0;
		char c;
		while (true) {
			c = getChar();
			lineN++;
			if (c == '\r' || c == '\n') {
				int i = 0;
				while (!inputQueue.empty()) {
					word[i++] = inputQueue.front();
					inputQueue.pop();
				}
				word[i] = 0;
				processWord();
				numWord++;
				lineN = 0;
				level = 0;
			}
			else if (c == '#'&&lineN==1) {
				c = getChar();
				int j = 0;
				switch (c) {
				case 'A':
					for (int i = 0; i < 7; i++)
						testSpace[i]=getChar();
					testSpace[7] = 0;

					if (strcmp(testSpace, "rticle:") != 0)
						break;

					c=getChar();
					do {
						c=getChar();
						if (c == '\n' || c == '\r') {
							currentArticle[j] = 0;
							lineN = 0;
							level = 0;
							break;
						}
						currentArticle[j++] = c;
					} while (j<=1024);

					if (articleNumber >= 523000 && articleNumber <= 530000)
						continue;
					level = 1;
					processArticleTitle();
					break;
				case 'T':
					while (!(c == '\n' || c == '\r'))	
						c = getChar();
					break;
				case 'S':
					for (int i = 0; i < 14; i++)
						testSpace[i]=getChar();
					testSpace[14] = 0;
					if (strcmp("ubtitle level ", testSpace) == 0) {
						c=getChar();
						level = (c - 48);
						c=getChar();
					}
					break;
				}
			}

			else if (c >= 'A'&&c <= 'Z')
				inputQueue.push(c + 32);
			else if (c >= 'a'&&c <= 'z')
				inputQueue.push(c);
			else {
				int i = 0;
				if (inputQueue.size()>128) {
					while (!inputQueue.empty()) {
						inputQueue.pop();
					}
					continue;
				}
				while (!inputQueue.empty()) {
					word[i++] = inputQueue.front();
					inputQueue.pop();
				}
				word[i] = 0;
				processWord();
				numWord++;
			}
		}
	}
};

int main() {
	LexiconManager lexManager;
	DocumentManager docManager;
	SRQParser myParser(string("D:\\Workspace\\Wikipedia\\Parsed\\articles_in_plain_text.txt"), docManager,lexManager);
	cout << "Loaded!";
	cin.get();
	try {
		myParser.execute();
		cin.get();
	}
	catch (...) {
		cout << "OOPSz" << endl;
	}
	int temp;
	cin >> temp;
	return 0;
}