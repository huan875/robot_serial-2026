#ifndef SERIAL_PORT_MANAGER_HPP_
#define SERIAL_PORT_MANAGER_HPP_

#include <string>
#include <functional>
#include <thread>
#include <atomic>

class SerialPortManager
{
public:
    using DataCallback = std::function<void(const std::vector<uint8_t>&)>;

    SerialPortManager();
    ~SerialPortManager();

    bool Open(const std::string& port, int baudrate);
    void Close();
    bool IsOpen() const;

    bool Write(const std::vector<uint8_t>& data);
    bool Write(const std::string& data);

    void SetDataCallback(DataCallback callback);
    void StartReading();
    void StopReading();

private:
    int _fd;
    std::thread _read_thread;
    std::atomic<bool> _reading;

    void _ReadLoop();
    bool _ConfigurePort(const std::string& port, int baudrate);
};

#endif