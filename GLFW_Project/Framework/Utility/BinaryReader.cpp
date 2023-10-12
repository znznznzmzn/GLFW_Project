#include "../../Framework.h"

BinaryReader::BinaryReader(string const& path) { OpenFile(path); }
BinaryReader::~BinaryReader() { CloseFile(); }

void BinaryReader::OpenFile(string const& path) {
	if (file.is_open()) CloseFile(); // 이미 열려있다면 닫기
	file.open(path, ios::binary);
	if (!file.is_open()) cout << "Warning, wrong file path = " << path << endl;
}
void BinaryReader::CloseFile() { file.close(); }

string BinaryReader::ReadAll() {
	if (!file.is_open()) {
		cout << "Warning, file isn't opened" << endl;
		return string();
	}
	stringstream stream; 
	stream << file.rdbuf(); 
	return stream.str();
}
string BinaryReader::ReadLine() {
	string line;
	getline(file, line);
	line.erase(find(line.begin(), line.end(), '\n'));
	return line;
}

void BinaryReader::ReadByte(void* data, uint dataSize) { 
	file.read((char*)data, dataSize);
	char* sn = new char[1];
	file.read(sn, 1);
	delete[] sn;
}
