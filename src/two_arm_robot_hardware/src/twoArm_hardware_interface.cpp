#include "two_arm_robot_hardware/twoArm_hardware_interface.hpp"

namespace two_arm_hardware {

hardware_interface::CallbackReturn TwoArmHardwareInterface::on_init(const hardware_interface::HardwareInfo & info)
{
    if (hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS)
    {
        return hardware_interface::CallbackReturn::ERROR;
    }

    info_             = info;
    joint1_motor_id_  = std::stoi(info_.hardware_parameters["joint1_motor_id_"]);
    joint2_motor_id_  = std::stoi(info_.hardware_parameters["joint2_motor_id_"]);
    port_             = info_.hardware_parameters["dynamixel_port"];
    driver_           = std::make_shared<XL330Driver>(port_);

    return hardware_interface::CallbackReturn::SUCCESS;
}


hardware_interface::CallbackReturn TwoArmHardwareInterface::on_configure(const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;
    if (driver_->init() !=0) {
        return hardware_interface::CallbackReturn::ERROR;
    }
    return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn TwoArmHardwareInterface::on_activate(const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;

    set_state("arm_joint1/position", 0.0);
    set_state("arm_joint2/position", 0.0);
    set_state("arm_joint1/velocity", 0.0);
    set_state("arm_joint2/velocity", 0.0);
    driver_->activateWithPositionMode(joint1_motor_id_);
    driver_->activateWithPositionMode(joint2_motor_id_);
    return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn TwoArmHardwareInterface::on_deactivate (const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;
    driver_->deactivate(joint1_motor_id_);
    driver_->deactivate(joint2_motor_id_);
    return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::return_type TwoArmHardwareInterface::read (const rclcpp::Time & time, const rclcpp::Duration & period)
{
    (void)time;
    double Pos_1 =        driver_->getPositionRadian(joint1_motor_id_);
    double Pos_2 = -1.0 * driver_->getPositionRadian(joint2_motor_id_);
    // if (abs(left_vel) < 0.03) { left_vel = 0.0; }
    // if (abs(right_vel) < 0.03) { right_vel = 0.0; }
    set_state("arm_joint1/position", Pos_1);
    set_state("arm_joint2/position", Pos_2);

    return hardware_interface::return_type::OK;
}

hardware_interface::return_type TwoArmHardwareInterface::write(const rclcpp::Time & time, const rclcpp::Duration & period)
{
    (void)time;
    (void)period;
    double   join1_command   =        get_command("arm_joint1/position");
    double   join2_command   = -1.0 * get_command("arm_joint2/position");
    
    driver_->setTargetPositionRadian(joint1_motor_id_,  join1_command );
    driver_->setTargetPositionRadian(joint2_motor_id_,  join2_command );
 
    return hardware_interface::return_type::OK;
}

} // namespace two_arm_hardware

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(two_arm_hardware::TwoArmHardwareInterface, hardware_interface::SystemInterface)