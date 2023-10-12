#include "../../Framework.h"

BinaryWriter::BinaryWriter(string const& path) { OpenFile(path); }
BinaryWriter::~BinaryWriter() { CloseFile(); }

void BinaryWriter::OpenFile(string const& path) {
	if (file.is_open()) CloseFile(); // 이미 열려있다면 닫기
	file.open(path, ios::binary);
	if (!file.is_open()) cout << "Warning, wrong file path = " << path << endl;
}
void BinaryWriter::CloseFile() { file.close(); }

void BinaryWriter::WriteLine(string line) { 
	if (line.find("\n") == string::npos) line += "\n";
	file.write(line.c_str(), line.size());
}
void BinaryWriter::WriteByte(void* data, uint dataSize) { 
	file.write((char*)data, dataSize); 
	file << "\n";
}
