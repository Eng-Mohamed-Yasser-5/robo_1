#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
    MinimalPublisher() : Node("minimal_publisher"), count_(0)
    {
        publisher_ = create_publisher<std_msgs::msg::String>("/min_publisher", 10);
        timer_ = create_wall_timer(
            500ms, std::bind(&MinimalPublisher::timerCallback, this));

            RCLCPP_INFO(get_logger(), "Publishing at 2Hz");
    };

    void timerCallback()
    {
        auto msg = std_msgs::msg::String();
        msg.data = "Hello, world" + std::to_string(count_++); 
        publisher_ -> publish(msg);
    }

private:
    size_t count_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc,char * argv[])
{
    rclcpp::init(argc, argv);

    auto minimal_publisher_node = std::make_shared<MinimalPublisher>();

    rclcpp::spin(minimal_publisher_node);

    rclcpp::shutdown();

    return 0;
}