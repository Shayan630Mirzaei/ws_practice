#ifndef TWO_ARM_CONTROLLER_ALGO_HPP
#define TWO_ARM_CONTROLLER_ALGO_HPP

#include "controller_interface/controller_interface.hpp"
#include "robot_interfaces/msg/joint_current_pose.hpp"

using FloatArray = robot_interfaces::msg::JointCurrentPose;


namespace controller_algo {

class FirstOrderLowPassFilter : public controller_interface::ControllerInterface {

    public:
        FirstOrderLowPassFilter();
        controller_interface::InterfaceConfiguration command_interface_configuration() const override;
        controller_interface::InterfaceConfiguration state_interface_configuration() const override;
 
        controller_interface::CallbackReturn on_init() override;
        controller_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override;
        controller_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;
        // controller_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;
        controller_interface::return_type update(const rclcpp::Time & time, const rclcpp::Duration & period) override;

    protected:
        std::vector<std::string> joint_names_;
        std::string interface_name_;
        double coefficient_;

        std::vector<double> applicationCommand_;                                 // user send command via topic
        rclcpp::Subscription<FloatArray>::SharedPtr joints_command_subscriber_;  // so we subscribe to a topic , we build subscriber in on_configure of cpp file




}; // class

} // namespace

#endif