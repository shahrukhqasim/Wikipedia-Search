#pragma once

class Lexicon;

#include <hash_map>
#include "LexiconManager.h"

using namespace std;

class Lexicon
{
private:
	hash_map<char,Lexicon*>subs;
	unsigned int hash;
	unsigned int count;
public:
	Lexicon();
	virtual ~Lexicon();
	void add(string element, unsigned int hash, unsigned int count=0);
	unsigned int getHash(string element, bool inc = false);
	unsigned int getCount(string element);
	void update(LexiconManager&db, string s);
};
