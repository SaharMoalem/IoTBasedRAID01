#include <sstream>
#include <iomanip>

#include "logger.hpp"

using namespace ilrd;

/*********************************static variables*****************************/

std::string Logger::s_sev_lut[NUM_OF_SEVERITIES] = {"DEBUG", "WARNING", "ERROR"};

/*******************************Private Functions******************************/

Logger::Logger(): s_logger("logger.txt", std::ios::app), m_running(true)
{
    if(!s_logger.is_open())
    {
        throw std::runtime_error("Couldn't open Logger.");
    }

    m_thread = std::thread([this]{LoggerRun();});
}


void Logger::LoggerRun()
{
    Message log;

    while(m_running || !m_queue.IsEmpty())
    {
        m_queue.Pop(log);
        LogMessageTask(log);
    }
}

void Logger::LogMessageTask(Message& message)
{
    std::time_t now_time = std::chrono::system_clock::to_time_t(message.time_created);
    std::tm now_tm = *std::localtime(&now_time);

    // std::string format = std::format("{:02d}.{:02d}.{:04d} {:02d}:{:02d} [{}] {}",
    //     now_tm.tm_mday,
    //     now_tm.tm_mon + 1,
    //     now_tm.tm_year + 1900,
    //     now_tm.tm_hour,
    //     now_tm.tm_min,  
    //     s_sev_lut[message.sev],
    //     message.text);
    std::ostringstream format;

    format << std::setfill('0') << std::setw(2) << now_tm.tm_mday << '.'
        << std::setw(2) << (now_tm.tm_mon + 1) << '.'
        << std::setw(4) << (now_tm.tm_year + 1900) << ' '
        << std::setw(2) << now_tm.tm_hour << ':'
        << std::setw(2) << now_tm.tm_min << " ["
        << s_sev_lut[message.sev] << "] "
        << message.text;

    s_logger << format.str() /*<< format*/ << std::endl;
}

/*******************************API Functions**********************************/

Logger::~Logger()
{
    m_queue.Push({std::chrono::system_clock::now(), "Logger shut down", WARNING});
    m_running = false;
    m_thread.join();
}
void Logger::Log(const std::string& message, Severity sev)
{
    m_queue.Push({std::chrono::system_clock::now(), message, sev});
}