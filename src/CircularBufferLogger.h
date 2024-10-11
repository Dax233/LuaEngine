#ifndef CIRCULAR_BUFFER_LOGGER_H
#define CIRCULAR_BUFFER_LOGGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <csignal>
#include <sstream>
#include <iomanip>
#include <filesystem>

// ��־�ȼ�
enum class MsgLevel {
    INFO,
    WARNING,
    LogError,
    READ,
    WRITE
};

// �Զ������ݵ���Ŀ�ṹ��
struct CustomDataEntry {
    std::string title;
    std::string content;
};

// ��־��¼�ṹ��
struct LogRecord {
    long long timestamp; // �Ժ���Ϊ��λ��ʱ���
    std::string functionName;                 // ����������ѡ��
    MsgLevel level;                           // �ȼ�
    std::string message;                      // ��Ϣ
    std::vector<CustomDataEntry> customData;  // �Զ��������б�
};

class CircularBufferLogger {
public:
    CircularBufferLogger(size_t bufferSize);

    // ��¼����
    void logOperation(const std::string& functionName, MsgLevel level, const std::string& message, const std::vector<CustomDataEntry>& customData = {});

    // ����ʱ���������еļ�¼д��������ļ�
    void saveLogToFile() const;

private:
    size_t bufferSize_;
    std::vector<LogRecord> buffer_;
    size_t currentIndex_;

    // ��������źŴ�����
    static void signalHandler(int signal);

    // д���ַ������ļ�
    static void writeString(std::ofstream& outFile, const std::string& str);

    // д����־�ȼ����ļ�
    static void writeLogLevel(std::ofstream& outFile, MsgLevel level);

    // д���Զ��������б��ļ�
    static void writeCustomData(std::ofstream& outFile, const std::vector<CustomDataEntry>& customData);
};

#endif // CIRCULAR_BUFFER_LOGGER_H