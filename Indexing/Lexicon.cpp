#include "Lexicon.h"

#include "DatabaseManager.h"


Lexicon::Lexicon()
{
	hash = 0;
	count = 0;
}

Lexicon::~Lexicon() {
	while (subs.begin() != subs.end()) {
		hash_map<char, Lexicon*>::iterator it=subs.begin();
		delete it->second;
		subs.erase(it);
	}
}

void Lexicon::add(string element, unsigned int hash, unsigned int count) {
	if (element.size() == 0) {
		this->hash = hash;
		if (count == 0)
			this->count++;
		else {
			this->count = count;
		}
		return;
	}
	char first = element[0];
	string next = element.substr(1);
	
	hash_map<char, Lexicon*>::iterator it = subs.find(first);

	if (it == subs.end()) {
		Lexicon*newLexicon = new Lexicon();
		subs[first] = newLexicon;
		newLexicon->add(next, hash,count);
	}
	else {
		subs[first]->add(next, hash, count);
	}
}

unsigned int Lexicon::getHash(string element, bool inc) {
	if (element.size() == 0) {
		if (inc)
			count++;
		return hash;
	}

	hash_map<char, Lexicon*>::iterator it = subs.find(element[0]);
	if (it == subs.end()) {
		return 0;
	}
	return subs[element[0]]->getHash(element.substr(1),inc);
}

unsigned int Lexicon::getCount(string element) {
	if (element.size() == 0) {
		return count;
	}

	hash_map<char, Lexicon*>::iterator it = subs.find(element[0]);
	if (it == subs.end()) {
		return 0;
	}
	return subs[element[0]]->getCount(element.substr(1));
}

void Lexicon::update(LexiconManager&db, string s) {
	hash_map<char, Lexicon*>::iterator it = subs.begin();

	if (hash != 0) {
//		cout << "Traversing to " << s << endl;
		db.lexiconUpdateHelper(s,hash, count);
	}

	while (it != subs.end()) {
		it->second->update(db,s+it->first);
		it++;
	}
}