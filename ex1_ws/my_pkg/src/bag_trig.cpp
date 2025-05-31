#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "sysmonitor_interfaces/msg/sysmon.hpp"

/**
 * @file bag_trig.cpp
 * @brief Nodo ROS 2 che pubblica un Float64 costante su /test ogni volta
 *        che riceve un messaggio Sysmon su /system_info.
 *
 * Parametri ROS:
 *   - constant_value (double, default = 1.0) : valore da pubblicare.
 */

class BagTrigNode : public rclcpp::Node{
    public: 
    BagTrigNode() : 
    Node("bag_trig_node"){

        // Parametro con valore di default
        this->declare_parameter<double>("constant_value", 1.0);
        this->get_parameter("constant_value", constant_value);

        // Publisher: /test con history 10
        pub_ = this->create_publisher<std_msgs::msg::Float64>("test", 10);

        // Subscriber: /system_info con history 10
        sub_ = this->create_subscription<sysmonitor_interfaces::msg::Sysmon>(
        "system_info",
        10,
        std::bind(&BagTrigNode::callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(),
                    "BagTrigNode avviato: pubblico %.3f su /test ad ogni msg su /system_info",
                    constant_value);
    }

    private:

    // Callback chiamata per ogni messaggio Sysmon in ingresso
    void callback(const sysmonitor_interfaces::msg::Sysmon::SharedPtr)
    {
        std_msgs::msg::Float64 out;
        out.data = constant_value;
        pub_->publish(out);
    }

  // Membri
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_;
  rclcpp::Subscription<sysmonitor_interfaces::msg::Sysmon>::SharedPtr sub_;
  double constant_value{1.0};
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BagTrigNode>());
  rclcpp::shutdown();
  return 0;
}