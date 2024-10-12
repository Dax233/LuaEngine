#include "CircularBufferLogger.h"

// ���캯��
CircularBufferLogger::CircularBufferLogger(size_t bufferSize)
    : bufferSize_(bufferSize) {
    // ���� log Ŀ¼
    std::filesystem::create_directory("logs");
}

void CircularBufferLogger::logOperation(const std::string& scriptName, const std::string& functionName, MsgLevel level, const std::string& message, const std::vector<CustomDataEntry>& customData) {
    LogRecord record;
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    record.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    record.scriptName = scriptName;
    record.functionName = functionName;
    record.level = level;
    record.message = message;
    record.customData = customData;

    // ����¼��ӵ���Ӧ�ű��Ļ�����
    buffers_[scriptName].push_back(record);

    // ���������������С���Ƴ���ɵļ�¼
    if (buffers_[scriptName].size() > bufferSize_) {
        buffers_[scriptName].erase(buffers_[scriptName].begin());
    }
}

void CircularBufferLogger::saveLogToFile() const {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime;
    localtime_s(&localTime, &now);  // ʹ�� localtime_s

    std::stringstream filename;
    filename << "logs/log_" << std::put_time(&localTime, "%Y%m%d_%H%M%S") << ".crash";
    std::ofstream outFile(filename.str(), std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "�޷�������־�ļ�" << std::endl;
        return;
    }

    for (const auto& [scriptName, records] : buffers_) { // ����ÿ���ű��Ļ�����
        for (const auto& record : records) {
            if (record.message.empty()) continue;

        // ��ÿ����¼д��������ļ�
        outFile.write(reinterpret_cast<const char*>(&record.timestamp), sizeof(record.timestamp));
        writeString(outFile, record.scriptName);
        writeString(outFile, record.functionName);
        writeLogLevel(outFile, record.level);
        writeString(outFile, record.message);
        writeCustomData(outFile, record.customData);
    }
    }
    outFile.close();
    // ��ʾ��ʾ��֪ͨ�û���־�ѱ���
    std::string message = "�����������������������������¼�ѱ�������\n" + filename.str();
    MessageBoxA(NULL, message.c_str(), "LuaEngine������ʾ", MB_OK | MB_ICONERROR);
}

void CircularBufferLogger::writeString(std::ofstream& outFile, const std::string& str) {
    size_t length = str.size();
    outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
    outFile.write(str.c_str(), length);
}

void CircularBufferLogger::writeLogLevel(std::ofstream& outFile, MsgLevel level) {
    outFile.write(reinterpret_cast<const char*>(&level), sizeof(level));
}

void CircularBufferLogger::writeCustomData(std::ofstream& outFile, const std::vector<CustomDataEntry>& customData) {
    size_t dataSize = customData.size();
    outFile.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    for (const auto& entry : customData) {
        writeString(outFile, entry.title);
        writeString(outFile, entry.content);
    }
}
