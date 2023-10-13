#pragma once

class BinaryWriter {
private:
    ofstream file;
public:
    BinaryWriter() = default;
    BinaryWriter(string const& path);
    ~BinaryWriter();

    void OpenFile(string const& path); // ���� ����
    void CloseFile(); // ���� �ݱ�

    void WriteLine(string line); // \n ���� ���� ����
    void WriteLinePure(string line); // \n ���� ���� ����

    void WriteByte(void* data, uint dataSize); // \n ���� ����Ʈ ����
    void WriteBytePure(void* data, uint dataSize); // \n ���� ����Ʈ ����

    const int& GetLineIndex() { return file.cur; }

    bool isFileOpened() { return file.is_open(); } // ������ ���ȴ��� �ľ�
};