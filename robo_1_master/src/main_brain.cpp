#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MyNode : public rclcpp::Node {
public:
  MyNode() : Node("my_node") {
    // Publisher initialization
    publisher_ = this->create_publisher<std_msgs::msg::String>("main_topic", 10);

    // Subscriber initialization
    subscription_ = this->create_subscription<std_msgs::msg::String>("/min_publisher", 10, std::bind(&MyNode::callback, this, std::placeholders::_1));
  }

private:
  void callback(const std_msgs::msg::String::SharedPtr msg) {
    // Process the received message
    RCLCPP_INFO(this->get_logger(), "I heard: %s", msg->data.c_str());

    // Publish a new message
    auto message = std_msgs::msg::String();
    message.data = "Processed: " + msg->data;
    publisher_->publish(message);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::Node::SharedPtr node = std::make_shared<MyNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}