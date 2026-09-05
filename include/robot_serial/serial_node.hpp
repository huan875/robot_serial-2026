#ifndef SERIAL_NODE_HPP_
#define SERIAL_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/u_int8_multi_array.hpp>
#include "serial_port_manager.hpp"

class SerialNode : public rclcpp::Node
{
public:
    SerialNode();
    ~SerialNode();

private:
    SerialPortManager _serial_manager;

    // 发布者
    rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr _pub_serial_data;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr _pub_serial_string;

    // 订阅者
    rclcpp::Subscription<std_msgs::msg::UInt8MultiArray>::SharedPtr _sub_serial_cmd;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr _sub_serial_string;

    // 定时器（用于周期任务）
    rclcpp::TimerBase::SharedPtr _timer;

    // 参数
    std::string _serial_port;
    int _baudrate;
    std::string _pub_topic_data;
    std::string _pub_topic_string;
    std::string _sub_topic_cmd;
    std::string _sub_topic_string;

    void _InitParameters();
    void _InitSerialPort();
    void _OnSerialDataReceived(const std::vector<uint8_t>& data);
    void _OnCmdReceived(const std_msgs::msg::UInt8MultiArray::SharedPtr msg);
    void _OnStringReceived(const std_msgs::msg::String::SharedPtr msg);
    void _TimerCallback();
};

#endif