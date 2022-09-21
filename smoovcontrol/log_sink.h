//
//  log_sink.h
//  smoovcontrol
//
//  Created by Paul Zaremba on 9/17/22.
//

#ifndef log_sink_h
#define log_sink_h

#include <iostream>

#include "spdlog/sinks/base_sink.h"

template<typename Mutex>
class log_sink : public spdlog::sinks::base_sink <Mutex>
{
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {

    // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
    // msg.raw contains pre formatted log

    // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    std::cout << fmt::to_string(formatted);
    }

    void flush_() override
    {
       std::cout << std::flush;
    }
};

#endif
