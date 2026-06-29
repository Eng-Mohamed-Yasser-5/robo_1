#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"


std::string subscribe_topic_name = "/scan";


class MyNode : public rclcpp::Node {
public:
  MyNode() : Node("my_node") {
    // Publisher initialization
    publisher_ = this->create_publisher<std_msgs::msg::String>("main_topic", 10);

    // Subscriber initialization
    subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(subscribe_topic_name, 10, std::bind(&MyNode::callback, this, std::placeholders::_1));
  }

private:
   void callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
        float min_distance = std::numeric_limits<float>::infinity();
        int min_index = -1;

        for (size_t i = 0; i < msg->ranges.size(); ++i) {
            float r = msg->ranges[i];

            // Validate reading
            if (std::isnan(r) || std::isinf(r)) continue;
            if (r < msg->range_min || r > msg->range_max) continue;

            if (r < min_distance) {
                min_distance = r;
                min_index = static_cast<int>(i);
            }
        }

        if (min_index != -1) {
            // Compute angle of closest point
            float angle = msg->angle_min + min_index * msg->angle_increment;
            RCLCPP_INFO(this->get_logger(),
                        "Closest point: distance = %.3f m, angle = %.3f rad",
                        min_distance, angle);
        } else {
            RCLCPP_WARN(this->get_logger(), "No valid points in scan.");
        }
    }
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::Node::SharedPtr node = std::make_shared<MyNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}