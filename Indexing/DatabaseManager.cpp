#include "DatabaseManager.h"


DatabaseManager::DatabaseManager()
{
	cout << "Global pointer good to go " << endl;
}


DatabaseManager::~DatabaseManager()
{
	delete con;
}

void DatabaseManager::connect() {
	try {
		/* Create a connection */
		driver = get_driver_instance();
		assert (driver != NULL);
		con = driver->connect("tcp://localhost:3306", "root", "");
		/* Connect to the MySQL test database */
		con->setSchema("dsa");

		//stmt = con->createStatement();
		//stmt->execute("INSERT INTO `dictionary`(`wordText`) VALUES ('work')");
		//delete stmt;
		//stmt = con->createStatement();
		//res=stmt->executeQuery("SELECT LAST_INSERT_ID() as id");
		//while (res->next()) {
		//	cout << "\t... MySQL replies: ";
		//	/* Access column data by alias or column name */
		//	cout << res->getInt("id") << endl;
		//}
		//delete res;
		//delete stmt;

	}
	catch (sql::SQLException &e) {
		cout << "[ERROR IN CONNECTING]" << endl;
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

}

void DatabaseManager::loadWords(Lexicon&lexicon) {
	cout << "Loading words" << endl;
	ifstream fileIn("D:\\Workspace\\Wikipedia\\Parser\\Lexicon.txt");
	int wordId;
	string word;
	int wordCount;
	while (fileIn>>wordId>>word>>wordCount) {
		cout << "Loading " << wordId << ". " << word << " " << wordCount << endl;
		lexicon.add(word, wordId, wordCount);
	}

	//sql::Statement *stmt;
	//sql::ResultSet *res;
	//stmt = con->createStatement();
	//res=stmt->executeQuery("SELECT * FROM `dictionary`");
	//while (res->next()) {
	//	string word = res->getString("wordText");
	//	int wordId = res->getInt("wordId");
	//	int wordCount = res->getInt("count");
	//	lexicon.add(word, wordId,wordCount);
	//	cout << "Added " << word << " to " << wordId<<" having count "<<wordCount << endl;
	//}
	//delete res;
	//delete stmt;
}


//void DatabaseManager::updateWord(unsigned int wordId, unsigned int wordCount) {
//	sql::Statement *stmt;
//	sql::ResultSet *res;
//	stmt = con->createStatement();
//	string s = string("UPDATE `dictionary` SET `count`=");
//	s.append(to_string(wordCount));
//	s.append(" WHERE wordId=");
//	s.append(to_string(wordId));
//	stmt->execute(s);
//	delete stmt;
//}

int DatabaseManager::addNewDocument(string document) {
	sql::Statement *stmt;
	sql::ResultSet *res;
	stmt = con->createStatement();
	string statement = "INSERT INTO `urls`(`url`) VALUES ('"+document+"')";
	stmt->execute(statement);
	delete stmt;
	stmt = con->createStatement();
	res=stmt->executeQuery("SELECT LAST_INSERT_ID() as id");
	int docId = -1;
	while (res->next()) {
		cout << "DocId is ";
		/* Access column data by alias or column name */
		docId=res->getInt("id");
	}
	delete res;
	delete stmt;
	return docId;
}

int DatabaseManager::addNewWord(string word) {
	sql::Statement *stmt;
	sql::ResultSet *res;
	stmt = con->createStatement();
	string statement = "INSERT INTO `dictionary`(`wordText`) VALUES ('" + word+ "')";
	stmt->execute(statement);
	delete stmt;
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT LAST_INSERT_ID() as id");
	int wordId = -1;
	while (res->next()) {
		/* Access column data by alias or column name */
		wordId = res->getInt("id");
	}
	delete res;
	delete stmt;
	return wordId;
}


void DatabaseManager::forwardWord(int wordId, int numWord, int docId, string table) {
	sql::Statement *stmt;
	sql::ResultSet *res;
	stmt = con->createStatement();
	string s = string("INSERT INTO `");
	s += table;
	s += "`(`docId`,`wordId`,`wordPosition`) VALUES('";
	s += to_string(docId) + "','" + to_string(wordId);
	s += "','" + to_string(numWord);
	s+="')";
	//cout << "Forwarding " << wordId << " having count " << numWord << " to " << docId << " in table " << table << endl;
	stmt->execute(s);
	delete stmt;
}