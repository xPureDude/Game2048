#include "log.hpp"

#include "../pch.hpp" // IWYU pragma: keep

// #include <chrono>

Log* Log::s_log = nullptr;

Log::Log()
    : m_log2file(false)
{
}

Log::~Log()
{
    if (true == m_log2file && m_log_file.is_open() == true)
        m_log_file.close();
}

void Log::init()
{
    if (true == m_log2file)
    {
        m_log_file.open("log.txt", std::ios::app);
    }
}

Log* Log::instance()
{
    if (s_log == nullptr)
        s_log = new Log;
    return s_log;
}

void Log::writeLog(const std::string& str)
{
    /*
    using namespace std::chrono;
    time_point<std::chrono::system_clock> current = system_clock::now();
    system_clock::duration current_since_epoch = current.time_since_epoch();


    long long duration = duration_cast<milliseconds>(current_since_epoch).count() -
        duration_cast<seconds>(current_since_epoch).count() * 1000;

    tm current_tm;
    time_t current_tmt = system_clock::to_time_t(current);
    localtime_s(&current_tm, &current_tmt);

    char time_str[32];
    sprintf_s(time_str, "%04d-%02d-%02d %02d:%02d:%02d",
        1900 + current_tm.tm_year, 1 + current_tm.tm_mon, current_tm.tm_mday,
        current_tm.tm_hour, current_tm.tm_min, current_tm.tm_sec);
    */

    // std::string log_str = std::format("{} {} \n", time_str, str);

    std::cout << str << std::endl;

    if (true == m_log2file)
        m_log_file << str << std::flush;
}

std::string ParseSrcFileName(std::string_view str)
{
    for (std::size_t i = str.size() - 1; i != 0; --i)
    {
        if (str[i] == '\\')
        {
            return str.substr(i + 1).data();
        }
    }
    return str.data();
}