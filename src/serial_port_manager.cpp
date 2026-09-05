#include "robot_serial/serial_port_manager.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

SerialPortManager::SerialPortManager()
    : _fd(-1), _reading(false)
{
}

SerialPortManager::~SerialPortManager()
{
    Close();
}

bool SerialPortManager::Open(const std::string& port, int baudrate)
{
    _fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (_fd < 0)
    {
        std::cerr << "Failed to open port: " << port << std::endl;
        return false;
    }

    if (!_ConfigurePort(port, baudrate))
    {
        close(_fd);
        _fd = -1;
        return false;
    }

    std::cout << "Serial port opened: " << port << " at " << baudrate << " bps" << std::endl;
    return true;
}

bool SerialPortManager::_ConfigurePort(const std::string& port, int baudrate)
{
    struct termios tty;
    if (tcgetattr(_fd, &tty) != 0)
    {
        std::cerr << "Error getting terminal attributes" << std::endl;
        return false;
    }

    speed_t speed;
    switch (baudrate)
    {
    case 9600: speed = B9600; break;
    case 19200: speed = B19200; break;
    case 38400: speed = B38400; break;
    case 57600: speed = B57600; break;
    case 115200: speed = B115200; break;
    default: speed = B115200; break;
    }

    cfsetispeed(&tty, speed);
    cfsetospeed(&tty, speed);

    // 8N1: 8 data bits, no parity, 1 stop bit
    tty.c_cflag &= ~PARENB;        // No parity
    tty.c_cflag &= ~CSTOPB;        // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;            // 8 data bits
    tty.c_cflag &= ~CRTSCTS;       // No flow control

    // 非规范模式
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(_fd, TCSANOW, &tty) != 0)
    {
        std::cerr << "Error setting terminal attributes" << std::endl;
        return false;
    }

    return true;
}

void SerialPortManager::Close()
{
    StopReading();
    if (_fd >= 0)
    {
        close(_fd);
        _fd = -1;
        std::cout << "Serial port closed" << std::endl;
    }
}

bool SerialPortManager::IsOpen() const
{
    return _fd >= 0;
}

bool SerialPortManager::Write(const std::vector<uint8_t>& data)
{
    if (!IsOpen())
        return false;

    ssize_t written = write(_fd, data.data(), data.size());
    return written == (ssize_t)data.size();
}

bool SerialPortManager::Write(const std::string& data)
{
    if (!IsOpen())
        return false;

    ssize_t written = write(_fd, data.c_str(), data.size());
    return written == (ssize_t)data.size();
}

void SerialPortManager::SetDataCallback(DataCallback callback)
{
    // 这里可以存储callback供_ReadLoop使用
    // 为简洁起见，直接在_ReadLoop中实现
}

void SerialPortManager::StartReading()
{
    if (_reading)
        return;

    _reading = true;
    _read_thread = std::thread(&SerialPortManager::_ReadLoop, this);
}

void SerialPortManager::StopReading()
{
    _reading = false;
    if (_read_thread.joinable())
        _read_thread.join();
}

void SerialPortManager::_ReadLoop()
{
    uint8_t buffer[256];
    while (_reading && IsOpen())
    {
        ssize_t n = read(_fd, buffer, sizeof(buffer));
        if (n > 0)
        {
            std::vector<uint8_t> data(buffer, buffer + n);
            // 数据通过ROS发布
        }
        usleep(10000); // 10ms 延迟
    }
}