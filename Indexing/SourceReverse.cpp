#include <iostream>
#include <fstream>
#include <string>
#include <hash_set>
#include <hash_map>

using namespace std;

void cacheFlush(hash_map<int, hash_map<int, int>*>&cache) {
	char * ccache = new char[33554432];
	for (hash_map<int, hash_map<int, int>*>::iterator iter = cache.begin(); iter != cache.end(); ++iter) {
		ofstream ofile("D:\\Workspace\\Wikipedia\\Parser\\S\\title1\\"+to_string(iter->first),ios::app|ios::binary);
		hash_map<int, int>*docIds = iter->second;
		int len = 0;
		for (hash_map<int, int>::iterator iterR = docIds->begin(); iterR != docIds->end(); ++iterR) {
			int temp = iterR->first;
			int t2 = iterR->second;
			const char*tempP = reinterpret_cast<const char*>(&temp);
			const char*tempR = reinterpret_cast<const char*>(&t2);
			for (int i = 0; i < 4; i++) {
				ccache[len++] = tempP[i];
			}
			for (int i = 0; i < 4; i++) {
				ccache[len++] = tempR[i];
			}
		}
		ofile.write(ccache, len);
		delete docIds;
		ofile.close();
	}
	cache.clear();
	delete [] ccache;
}


int main() {
	ifstream file("D:\\Workspace\\Wikipedia\\Parser\\ForwardTitles.txt", ios::binary);
	int docId, wordId;

	hash_map<int, hash_map<int, int>*> cache;

	int i = 0;
	while (!file.eof()) {
		file.read(reinterpret_cast<char*>(&docId), sizeof(int));
		file.read(reinterpret_cast<char*>(&wordId), sizeof(int));
		hash_map<int, hash_map<int,int>*>::iterator it = cache.find(wordId);
		if (it==cache.end()) {
			hash_map<int, int>*newSet = new hash_map<int, int>;
			(*newSet)[docId] = 1;
			cache[wordId] = newSet;
		}
		else {
			hash_map<int, int>*newSet = it->second;
			
			hash_map<int, int>::iterator zt = newSet->find(docId);
			if (zt == newSet->end()) {
				(*newSet)[docId] = 1;
			}
			else {
				(*newSet)[docId]++;
			}
		}
		if (i % 20000000==0) {
			cacheFlush(cache);
		}
		i++;
	}
	cout << "Ready to flush cache" <<cache.size()<< endl;
	cin.get();
	cacheFlush(cache);
	cout << "Ready! "<<i<< endl;
	cin.get();
	return 0;
}