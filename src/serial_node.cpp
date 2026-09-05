#include "robot_serial/serial_node.hpp"

SerialNode::SerialNode()
    : Node("serial_node")
{
    _InitParameters();
    _InitSerialPort();

    // 发布者
    _pub_serial_data = this->create_publisher<std_msgs::msg::UInt8MultiArray>(
        _pub_topic_data, 10);
    _pub_serial_string = this->create_publisher<std_msgs::msg::String>(
        _pub_topic_string, 10);

    // 订阅者
    _sub_serial_cmd = this->create_subscription<std_msgs::msg::UInt8MultiArray>(
        _sub_topic_cmd, 10,
        std::bind(&SerialNode::_OnCmdReceived, this, std::placeholders::_1));

    _sub_serial_string = this->create_subscription<std_msgs::msg::String>(
        _sub_topic_string, 10,
        std::bind(&SerialNode::_OnStringReceived, this, std::placeholders::_1));

    // 定时器
    _timer = this->create_wall_timer(
        std::chrono::milliseconds(100),
        std::bind(&SerialNode::_TimerCallback, this));

    RCLCPP_INFO(this->get_logger(), "Serial node initialized");
}

SerialNode::~SerialNode()
{
    _serial_manager.Close();
}

void SerialNode::_InitParameters()
{
    this->declare_parameter("port", "/dev/board_a");
    this->declare_parameter("baudrate", 115200);
    this->declare_parameter("pub_topic_data", "/serial/data");
    this->declare_parameter("pub_topic_string", "/serial/string");
    this->declare_parameter("sub_topic_cmd", "/serial/cmd");
    this->declare_parameter("sub_topic_string", "/serial/send");

    _serial_port = this->get_parameter("port").as_string();
    _baudrate = this->get_parameter("baudrate").as_int();
    _pub_topic_data = this->get_parameter("pub_topic_data").as_string();
    _pub_topic_string = this->get_parameter("pub_topic_string").as_string();
    _sub_topic_cmd = this->get_parameter("sub_topic_cmd").as_string();
    _sub_topic_string = this->get_parameter("sub_topic_string").as_string();

    RCLCPP_INFO(this->get_logger(), "Port: %s, Baudrate: %d",
        _serial_port.c_str(), _baudrate);
}

void SerialNode::_InitSerialPort()
{
    if (!_serial_manager.Open(_serial_port, _baudrate))
    {
        RCLCPP_ERROR(this->get_logger(), "Failed to open serial port");
        return;
    }

    _serial_manager.StartReading();
}

void SerialNode::_OnCmdReceived(const std_msgs::msg::UInt8MultiArray::SharedPtr msg)
{
    _serial_manager.Write(msg->data);
    RCLCPP_DEBUG(this->get_logger(), "Sent %zu bytes", msg->data.size());
}

void SerialNode::_OnStringReceived(const std_msgs::msg::String::SharedPtr msg)
{
    _serial_manager.Write(msg->data);
    RCLCPP_DEBUG(this->get_logger(), "Sent string: %s", msg->data.c_str());
}

void SerialNode::_TimerCallback()
{
    // 周期任务（如心跳检测等）
}