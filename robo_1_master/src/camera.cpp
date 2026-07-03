#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.hpp"
#include "opencv2/opencv.hpp"


// Minimal openCV ros2 node

// To use external camera uncomment these two lines.  -- use droidcam to use your phone camera as a usb/wifi camere
// cv::Mat frame;
// cv::VideoCapture cam(0);

class CameraNode : public rclcpp::Node
{
public:

    CameraNode()
    : Node("camera_node")
    {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>("/camera/image_raw", 10, std::bind( &CameraNode::imageCallback, this, std::placeholders::_1));
    }

private:

    void imageCallback(
        const sensor_msgs::msg::Image::SharedPtr msg)
    {
        try
        {
            // get image from msg from "/camera/image_raw"
            cv::Mat frame = cv_bridge::toCvCopy(msg, "bgr8")->image;

            // To use external camera uncomment this line
            // cam.read(frame);


            // draw a circle
            int center_x = frame.cols/2;
            int center_y = frame.rows/2;
            cv::circle(frame, cv::Point(center_x,center_y), 20, cv::Scalar(0,255,0), 3);

            // show the frames after proccesing
            cv::imshow("camera", frame);

            cv::waitKey(1);
        }
        catch(cv_bridge::Exception &e)
        {
            RCLCPP_ERROR(this->get_logger(), "Error: %s", e.what());
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);

    auto node = std::make_shared<CameraNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}