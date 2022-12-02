// Copyright Heinz-Peter Liechtenecker 2020
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <mutex>

#include <atomic>
#include <thread>
#include "serial/serial.h"
#include "Common.h"

namespace serialcsv
{
    using serial::bytesize_t;
    using serial::parity_t;
    using serial::stopbits_t;
    using serial::flowcontrol_t;

    struct SerialConfig
    {
        SerialConfig(const std::string& p, uint32_t baud = 115200) 
        {
            port = p;
            eol = "\n";
            baudrate = baud;
            bytesize = bytesize_t::eightbits;
            parity = parity_t::parity_none;
            stopbits = stopbits_t::stopbits_one;
            flowcontrol = flowcontrol_t::flowcontrol_none;
        }

        std::string port;               // The Port to open
        std::string eol;                // The End-Of-Line Character
        uint32_t baudrate;              
        bytesize_t bytesize;            
        parity_t parity;                
        stopbits_t stopbits;            
        flowcontrol_t flowcontrol;
    };

    class SerialPort
    {
    public:
        using SerialLineCallback = std::function<void(const std::string&)>;

        SerialPort();
        ~SerialPort();
        std::vector<std::string> enumeratePorts();
        void setTimeout(const serial::Timeout& timeout);
        void start(const SerialConfig &portname, const SerialLineCallback& cb);
        void stop();
        bool isOpen();
        bool writeLn(const std::string& m);

    private:
        void notifyAvailable();
        bool ensureOpenedPort();
        bool readLine(std::string &line);
        void closePort();

        std::unique_ptr<serial::Serial> m_serial;
        SerialConfig m_config;
        SerialLineCallback m_callback;
        serial::Timeout m_timeout;
        std::thread m_thread;
        std::atomic<bool> m_stop_token;
        std::mutex m_mutex_port;
        bool m_started;
        bool m_stop;
        std::string m_buf;
    };
} // namespace serialcsv
