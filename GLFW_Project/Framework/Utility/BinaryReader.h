#pragma once

class BinaryReader {
private:
	ifstream file;
public:
	BinaryReader() = default;
	BinaryReader(string const& path);
	~BinaryReader();

	void OpenFile(string const& path);
	void CloseFile();

	string ReadAll();
	string ReadLine();
	void ReadByte(void* data, uint dataSize);

	const int& GetLineIndex() { return file.cur; }

	bool isFileOpened() { return file.is_open(); }
};