#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"


class MyNode : public rclcpp::Node {
public:
  MyNode() : Node("controller_node") {
    // Publisher initialization
    // ros2 topic pub /controller std_msgs/msg/String "{data: '0.5'}"
    // to turn the robor with angular velocity of 0.5 rad/s
    // also can use custom msgs
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // Subscriber initialization
    subscription_ = this->create_subscription<std_msgs::msg::String>("/controller", 10, std::bind(&MyNode::callback, this, std::placeholders::_1));
  }

private:
  void callback(const std_msgs::msg::String::SharedPtr msg) {
    // Process the received message
    RCLCPP_INFO(this->get_logger(), "I heard: %s", msg->data.c_str());

    // Publish a new message
    auto message = geometry_msgs::msg::Twist();
    // message.linear.x = int(msg->data.c_str());
    message.linear.x = 0;
    message.angular.z = std::stod(msg->data);
    publisher_->publish(message);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::Node::SharedPtr node = std::make_shared<MyNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}