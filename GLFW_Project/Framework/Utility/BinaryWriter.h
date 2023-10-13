#pragma once

class BinaryWriter {
private:
    ofstream file;
public:
    BinaryWriter() = default;
    BinaryWriter(string const& path);
    ~BinaryWriter();

    void OpenFile(string const& path); // 파일 열기
    void CloseFile(); // 파일 닫기

    void WriteLine(string line); // \n 포함 한줄 쓰기
    void WriteLinePure(string line); // \n 없이 한줄 쓰기

    void WriteByte(void* data, uint dataSize); // \n 포함 바이트 쓰기
    void WriteBytePure(void* data, uint dataSize); // \n 없이 바이트 쓰기

    const int& GetLineIndex() { return file.cur; }

    bool isFileOpened() { return file.is_open(); } // 파일이 열렸는지 파악
};