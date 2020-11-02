#include "serialcsvplugin/details/SerialPort.h"
using namespace serialcsv;

SerialPort::SerialPort() : m_started(false),
                           m_stop(false),
                           m_serial(nullptr),
                           m_config(""),
                           m_buf{}
{
}

SerialPort::~SerialPort()
{
    stop();
}

std::vector<std::string> SerialPort::enumaratePorts()
{
    std::vector<std::string> ret;
    std::vector<serial::PortInfo> devices_found = serial::list_ports();

    for (const auto &device : devices_found)
    {
        ret.push_back(device.port);
    }

    return ret;
}

void SerialPort::start(const SerialConfig &config, const SerialLineCallback &cb)
{
    // Ensure Worker Thread is stopped and Port is closed
    stop();

    m_config = config;
    m_callback = cb;

    m_thread = std::jthread([&](std::stop_token st) {
        while (!st.stop_requested())
        {
            if (ensureOpenedPort())
            {
                try
                {
                    m_serial->readline(m_buf, 65536, m_config.eol);
                    if (m_buf.find(m_config.eol) != std::string::npos)
                    {
                        m_callback(m_buf);
                        m_buf.clear();
                    }
                }
                catch (const std::exception&)
                {
                    // Fix unplugging usb serial port
                    m_serial->close();
                    m_serial.reset();

                    // do not leave thread but wait for device plugged in again
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
    });

    m_started = true;
}

void SerialPort::stop()
{
    if (!m_started)
    {
        return;
    }

    m_thread.request_stop();
    m_thread.join();

    closePort();
    m_started = false;
}

bool SerialPort::ensureOpenedPort()
{
    std::unique_lock<std::mutex> lock(m_mutex_port);
    if (!m_serial)
    {
        try
        {
            // Create the Serial Port
            m_serial = std::make_unique<serial::Serial>(
                m_config.port,
                m_config.baudrate,
                serial::Timeout::simpleTimeout(250),
                m_config.bytesize,
                m_config.parity,
                m_config.stopbits,
                m_config.flowcontrol);

            m_serial->flush();
            m_serial->flushInput();
            m_serial->flushOutput();
        }
        catch (const std::invalid_argument &)
        {
            // Log error?
            return false;
        }
        catch (const serial::SerialException &)
        {
            // Log error?
            return false;
        }
        catch (const serial::IOException &)
        {
            // Log error?
            return false;
        }
    }

    if (!m_serial->isOpen())
    {
        try
        {
            m_serial->open();
            m_serial->flush();
            m_serial->flushInput();
            m_serial->flushOutput();
        }
        catch (const std::invalid_argument &)
        {
            // Log error?
            return false;
        }
        catch (const serial::SerialException &)
        {
            // Log error?
            return false;
        }
        catch (const serial::IOException &)
        {
            // Log error?
            return false;
        }
    }

    return true;
}

void SerialPort::closePort()
{
    if (!m_serial)
    {
        return;
    }

    if (m_serial->isOpen())
    {
        m_serial->close();
    }

    m_serial.reset();
    m_serial = nullptr;
}

bool SerialPort::isOpen()
{
    std::unique_lock<std::mutex> lock(m_mutex_port);
    if (m_serial)
    {
        return m_serial->isOpen();
    }
    else
    {
        return false;
    }
}

bool SerialPort::writeLn(const std::string &m)
{
    if (!m_serial)
    {
        return false;
    }

    if (!ensureOpenedPort())
    {
        return false;
    }

    try
    {
        m_serial->write(m + m_config.eol);
    }
    catch (const serial::PortNotOpenedException e)
    {
        // Log Error?
        return false;
    }
    catch (const serial::IOException e)
    {
        // Log Error?
        return false;
    }
    return true;
}
