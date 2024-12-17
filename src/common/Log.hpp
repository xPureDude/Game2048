#if !defined(LOG_HPP)
#define LOG_HPP
/* ==========================================================================

    $File: log.hpp  $
    $Date:   $
    $Revision:   $
    $Author: Meta-X  $
    $Notice: (C) Copyright 2023 by Meta-X. All Rights Reserved.  $

   ========================================================================== */

#include <fstream>
#include <string>
#include <format>

std::string ParseSrcFileName(std::string_view str);

#define INF(...) Log::instance()->writeLog(std::format("[INFO ] [{}:{}] {}", ParseSrcFileName(__FILE__), __LINE__, std::format(__VA_ARGS__)))
#define DBG(...) Log::instance()->writeLog(std::format("[DEBUG] [{}:{}] {}", ParseSrcFileName(__FILE__), __LINE__, std::format(__VA_ARGS__)))
#define ERR(...) Log::instance()->writeLog(std::format("[ERROR] [{}:{}] {}", ParseSrcFileName(__FILE__), __LINE__, std::format(__VA_ARGS__)))

class Log
{
public:
    static Log* instance();

    ~Log();
    void init();
    
    void writeLog(const std::string& str);

protected:
    Log();

    static Log* s_log;

    bool m_log2file;
    std::ofstream m_log_file;
};

#endif //LOG_H
