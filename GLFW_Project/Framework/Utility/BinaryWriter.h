#pragma once

class BinaryWriter {
private:
    ofstream file;
public:
    BinaryWriter() = default;
    BinaryWriter(string const& path);
    ~BinaryWriter();

    void OpenFile(string const& path);
    void CloseFile();

    void WriteLine(string line);
    void WriteByte(void* data, uint dataSize);

    const int& GetLineIndex() { return file.cur; }

    bool isFileOpened() { return file.is_open(); }
};