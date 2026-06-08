#ifndef TWO_ARM_HARDWARE_INTERFACE_HPP
#define TWO_ARM_HARDWARE_INTERFACE_HPP

#include "hardware_interface/system_interface.hpp"
#include "two_arm_robot_hardware/xl330_driver.hpp"

namespace two_arm_hardware {

class TwoArmHardwareInterface : public hardware_interface::SystemInterface
{
public:
    // Lifecycle node override
    hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override;
    hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;
    hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

    // SystemInterface override
    hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;
    hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;
    hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
    std::shared_ptr<XL330Driver> driver_;
    int joint1_motor_id_;
    int joint2_motor_id_;
    std::string port_;

}; // class MobileBaseHardwareInterface

} // namespace two_arm_hardware 


#endif