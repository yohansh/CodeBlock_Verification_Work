//INCLUDES fstream for file system, sqlite.h for c++ interface to sqlite, tinyxml2.h for parsing of xml code with tinyxml2. 
//Compiled using sqlite.o file, please check the run.bat file on how to complie
//or use parse.exe to simply run what I have prepared.
#include "block.h"
#include "../../../../../sqlite-amalgamation-3300100/sqlite3.h"//Path to sqlite3.h header file.
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> 


using namespace std;
using namespace tinyxml2;


const char* gszFile = "swerv11_db.sqlite";


//Vector of Block objects.
vector<Block> vBlock;

//Using tinyxml2 to parse xmlcode String
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

string HexToBin(string hexdec)
{
	long int i = 0;
	string hexToBinary;

	while (hexdec[i]) {

		switch (hexdec[i]) {
		case '0':
			hexToBinary += "0000";
			break;
		case '1':
			hexToBinary += "0001";
			break;
		case '2':
			hexToBinary += "0010";
			break;
		case '3':
			hexToBinary += "0011";
			break;
		case '4':
			hexToBinary += "0100";
			break;
		case '5':
			hexToBinary += "0101";
			break;
		case '6':
			hexToBinary += "0110";
			break;
		case '7':
			hexToBinary += "0111";
			break;
		case '8':
			hexToBinary += "1000";
			break;
		case '9':
			hexToBinary += "1001";
			break;
		case 'A':
		case 'a':
			hexToBinary += "1010";
			break;
		case 'B':
		case 'b':
			hexToBinary += "1011";
			break;
		case 'C':
		case 'c':
			hexToBinary += "1100";
			break;
		case 'D':
		case 'd':
			hexToBinary += "1101";
			break;
		case 'E':
		case 'e':
			hexToBinary += "1110";
			break;
		case 'F':
		case 'f':
			hexToBinary += "1111";
			break;
		default:
			cout << "\nInvalid hexadecimal digit " << hexdec[i];
		}
		i++;
	}
	return hexToBinary;
}



//Loading specific lines from specific files.
string snippet(string path, int start, int end) {
	string tempSnippet;
	string line;
	ifstream file(path);
	if (file.is_open()) {
		for (int lineno = 1; getline(file, line); lineno++) {
			string processedLine = "";
			if (lineno >= start && lineno <= end) {
				for (int i = 0; i < line.size(); i++) {
					if (line[i] != '/' && line[i] != ' ' && line[i] != '(' && line[i] != ')') {
						processedLine += line[i];
					}
					if (line[i] == '/') {
						if (i + 1 < line.size() && line[i + 1] == '/') {
							break;
						}
					}
				}
				tempSnippet += processedLine;
				processedLine = "";
			}
		}
		file.close();
	}
	else {
		std::cout << "Can't Open File" << endl;
	}
	return tempSnippet;

}

//Pattern Matching
bool patternMatching(vector<string>& xmlCode, string& filePattern) {
	int pos = 0;
	int index;
	for (auto itr = xmlCode.begin(); itr != xmlCode.end(); ++itr) {
		//cout << "Check if str1 begins with: " << *itr << endl;

		string tempXMLNumber = (string)*itr;
		int pos = 0;
		int bitSize = 0;
		while (isdigit(tempXMLNumber[pos])) {
			bitSize = 10 * bitSize + (tempXMLNumber[pos] - '0');
			pos++;
		}
		if (tempXMLNumber[pos] == '\'' && bitSize > 0) {
			//cout << "We found a number in: " << tempXMLNumber << endl;
			string xmlStringValue = "";
			for (int i = pos + 2; i < tempXMLNumber.size(); i++) {
				xmlStringValue += tempXMLNumber[i];
			}
			char filePatternRedix;
			string filePatternValue = "";
			string tempFilePatternNumber = filePattern;
			int posINFilePattern = 0;
			int bitSizeINFilePattern = 0;
			while (isdigit(tempFilePatternNumber[posINFilePattern])) {
				bitSizeINFilePattern = 10 * bitSizeINFilePattern + (tempFilePatternNumber[posINFilePattern] - '0');
				posINFilePattern++;
			}
			//cout << bitSize << " * " << bitSizeINFilePattern << endl;
			if (tempFilePatternNumber[posINFilePattern] != '\'') {
				return false;//Not Matched
			}
			posINFilePattern++;
			filePatternRedix = tempFilePatternNumber[posINFilePattern];
			posINFilePattern++;
			while (isdigit(tempFilePatternNumber[posINFilePattern]) || isalpha(tempFilePatternNumber[posINFilePattern])) {
				filePatternValue += tempFilePatternNumber[posINFilePattern];
				posINFilePattern++;
			}
			//cout << tempXMLNumber[pos + 1] << " ** "<< filePatternRedix << endl;
			//Comparing xmlStringValue number and filePatternValue Number
			if (tempXMLNumber[pos + 1] == 'b' && filePatternRedix == 'b') {
				//cout << filePatternValue << " * " << xmlStringValue << endl;
				int posXML = 0;
				int posFile = 0;
				while (xmlStringValue[posXML] == '0') {
					posXML++;
				}
				while (filePatternValue[posFile] == '0') {
					posFile++;
				}
				xmlStringValue = xmlStringValue.substr(posXML);
				filePatternValue = filePatternValue.substr(posFile);
				if (xmlStringValue == filePatternValue) {
					filePattern = filePattern.substr(posINFilePattern);
				}
				else {
					return false;
				}
			}
			else if (tempXMLNumber[pos + 1] == 'b' && filePatternRedix == 'h') {
				//cout << "  **  "<<"A hex filePattern " <<endl;
				int posXML = 0;
				int posFile = 0;
				while (xmlStringValue[posXML] == '0') {
					posXML++;
				}
				string fileValueHexTODec = HexToBin(filePatternValue);
				//cout << xmlStringValue << " * " << fileValueHexTODec << endl;
				while (fileValueHexTODec[posFile] == '0') {
					posFile++;
				}
				xmlStringValue = xmlStringValue.substr(posXML);
				fileValueHexTODec = fileValueHexTODec.substr(posFile);
				//cout << xmlStringValue << " * " << fileValueHexTODec << endl;
				if (xmlStringValue == fileValueHexTODec) {
					filePattern = filePattern.substr(posINFilePattern);
					//cout << filePattern << endl;
				}
				else {
					return false;
				}
			}
			else {
				std::cout << "Not a binary its: " << tempXMLNumber[pos + 1] << endl;
				return false;
			}
		}
		else {
			if (*itr != "(" && *itr != ")" && (index = filePattern.find(*itr)) != string::npos) {
				//cout << "Match found at position: " << index << endl;
				if (index != 0) {
					return false; //code blocks don't match
				}
				filePattern = filePattern.substr(index + itr->size());
				//cout << " New string after deleting tokens: " << filePattern << endl;
			}
			else if (*itr == "(" || *itr == ")") {
				continue;
			}
			else {
				return false;
			}
		}
	}
	return true; //code blocks match
}

//Callback function of sqlite.
static int callback(void* data, int argc, char** argv, char** azColName)
{
	(void)data;
	(void)argc;
	(void)azColName;

	//Parsing all the nine fields in there format.
	//int id = stoi((string)argv[0]);
	//int groupId = stoi((string)argv[1]);
	string type = (string)argv[2];
	string sourceFileName = (string)argv[3];
	//string moduleName = (string)argv[4];
	int startLine = stoi((string)argv[5]);
	int endLine = stoi((string)argv[6]);
	//string verilogCode = (string)argv[7];
	string verilogCodeXml = (string)argv[8];

	//Creating object of class Block.
	Block tempBlock(/*id,*/ /*groupId,*/ type, sourceFileName, /*moduleName,*/ startLine, endLine, /*verilogCode,*/ verilogCodeXml);
	//Filling the vector of Block with above object.
	vBlock.push_back(tempBlock);

	return 0;
}

int main()
{
	//##Part 1 Parsing complete DB file rows as Block objects.  

	sqlite3* db;
	int exit = 0;

	//Probably loading/Opening the file as a reference in db;
	exit = sqlite3_open(gszFile, &db);

	if (exit) {
		cerr << "Error open db" << sqlite3_errmsg(db) << endl;
		return (-1);
	}
	else {
		std::cout << "Opened Code block Successfully" << endl;
	}

	//Query 
	string sql = "SELECT * FROM CodeBlocks;";

	//Execution of Query
	int rc = sqlite3_exec(db, sql.c_str(), callback, NULL, NULL);

	if (rc != SQLITE_OK) {
		cerr << "Error SELECT" << endl;
	}
	else {
		std::cout << "Operation OK!" << endl;
	}
	//Close
	sqlite3_close(db);

	//##
	int notMatchedCount = 0;
	int count = 0;
	//int blockNo = 46;
	//for (int blockNo = 0;blockNo < (int)vBlock.size(); blockNo++) {
	for (int blockNo = 0; blockNo < 500; blockNo++) {
		if (vBlock[blockNo].getType() == "ASSIGN" || vBlock[blockNo].getType() == "ALWAYS") {
			//##Part 2 Trying to pick the particular snippet of code that is to be administered.
			string filePattern = "";
			string pathToFile = vBlock[blockNo].getPath();
			//pathToFile format: ../cha_swerv_11_testgenvar/design/ifu/ifu_bp_ctl.sv
			pathToFile = pathToFile.substr(3);
			pathToFile = "../CopyOf_" + pathToFile;
			filePattern = snippet(pathToFile, vBlock[blockNo].getstartLine(), vBlock[blockNo].getendLine());
			//##

			//##Part 3 Parsing xml code out of db file row using tinyxml2;

			XMLDocument xmlDoc;
			string tempXml = vBlock[blockNo].getxmlCode();

			//Process to clean xmlCode as we had some spaces before starting of the code.
			int pos = 0;
			while (tempXml[pos] == ' ') {
				pos++;
			}
			tempXml = tempXml.substr(pos);
			//
			const char* xml = tempXml.c_str();
			xmlDoc.Parse(xml);//parsing string to XMLDocument
			XMLElement* pnode = xmlDoc.RootElement();

			if (pnode == nullptr)
			{
				std::cout << "no child node in Id: * " << blockNo << endl;
			}
			const char* textPointer = nullptr;
			//string xmlCode="";
			vector<string> xmlCode;
			while (pnode) {
				if (pnode->GetText() && (string)pnode->Value() != "comment") {
					textPointer = pnode->GetText();
					//xmlCode += textPointer;
					string tempTextPointer = textPointer;
					string proccessedTempTextPointer = "";
					for (auto i : tempTextPointer) {
						if (i != '(' && i != ')' && i != ' ') {
							proccessedTempTextPointer += i;
						}
					}
					xmlCode.push_back(proccessedTempTextPointer);
				}
				pnode = pnode->NextSiblingElement();
			}

			string xmlProccessedString = "";

			//Processing xmlString
			/*for (auto i : xmlCode) {
				if (i != '(' && i != ')'&&i!=' ') {
					xmlProccessedString += i;
				}
			}*/


			//Proccessing filePattern removing /t;
			filePattern.erase(std::remove(filePattern.begin(), filePattern.end(), '\t'), filePattern.end());



			//Pattern Matching
			//cout << filePattern << endl;
			string copyOfFilePattern = filePattern;
			string matched = patternMatching(xmlCode, filePattern) ? "Matched" : "Not Matched";
			if (matched == "Matched") {
				count++;
			}
			else {
				notMatchedCount++;
				std::cout << blockNo << " filePattern: " << copyOfFilePattern << endl;
				std::cout << blockNo << " remaining not Matched: " << filePattern << endl;
				std::cout << blockNo << " xmlCode: ";
				for (auto i : xmlCode) {
					std::cout << i << " ";
				}
				std::cout << endl;
				std::cout << "   ***  " << endl;
			}



			//if (filePattern == xmlProccessedString) {
			//	//cout << blockNo << ": " << "matched" << "   " << vBlock[i].getType() << endl;
			//	count++;
			//	//cout << blockNo << " " << "filePattern: " << processedFilePattern << endl;
			//	//cout << " xmlCode: " << xmlProccessedString << endl;
			//}
			//else {
			//	cout << blockNo << " " << "filePattern: " << filePattern << endl;
			//	cout << " xmlCode: " << xmlProccessedString << endl;
			//	//cout << blockNo << ": " << "not matched" << "   " << vBlock[i].getType() << endl;
			//	//count++;
			//}
			////cout << blockNo << ": " << matched << "   " <<vBlock[i].getType() << endl;
		}
	}
	std::cout << count << endl;
	cout << "Not Matched: " << notMatchedCount << endl;
	return XML_SUCCESS;
	//##
}
