#include "CircularBufferLogger.h"

CircularBufferLogger::CircularBufferLogger(size_t bufferSize)
    : bufferSize_(bufferSize), buffer_(bufferSize), currentIndex_(0) {
    // ����logĿ¼
    std::filesystem::create_directory("logs");

    // ���ñ���ʱ���źŴ�����
    signal(SIGSEGV, signalHandler);
}

void CircularBufferLogger::logOperation(const std::string& functionName, MsgLevel level, const std::string& message, const std::vector<CustomDataEntry>& customData) {
    LogRecord record;
    // ��ȡ��ǰʱ�䣨�������룩
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    record.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(); // �Ժ���Ϊ��λ��¼ʱ���
    record.functionName = functionName;
    record.level = level;
    record.message = message;
    record.customData = customData;

    // д�뻷�λ�����
    buffer_[currentIndex_] = record;
    currentIndex_ = (currentIndex_ + 1) % bufferSize_;  // ��������
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

    for (const auto& record : buffer_) {
        if (record.message.empty()) continue;

        // ��ÿ����¼д��������ļ�
        outFile.write(reinterpret_cast<const char*>(&record.timestamp), sizeof(record.timestamp));
        writeString(outFile, record.functionName);
        writeLogLevel(outFile, record.level);
        writeString(outFile, record.message);
        writeCustomData(outFile, record.customData);  // д���Զ�������
    }
    outFile.close();
}

void CircularBufferLogger::signalHandler(int signal) {
    // ������־���󲢱�����־
    CircularBufferLogger logger(100);
    logger.saveLogToFile();
    exit(signal);
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
