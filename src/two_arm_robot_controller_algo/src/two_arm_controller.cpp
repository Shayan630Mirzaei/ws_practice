#include "two_arm_robot_controller_algo/two_arm_controller_algo.hpp"



namespace controller_algo {

FirstOrderLowPassFilter::FirstOrderLowPassFilter():controller_interface::ControllerInterface()
{}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
controller_interface::CallbackReturn FirstOrderLowPassFilter::on_init()
{
    joint_names_     = auto_declare<std::vector<std::string>> ("joints",{});      // these paramteres can be configured in controller.ymal file
    interface_name_  = auto_declare<std::string>("interface_name","position");
    coefficient_     = auto_declare<double>("coefficient",0.8);

    return CallbackReturn::SUCCESS;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
controller_interface::CallbackReturn FirstOrderLowPassFilter::on_configure(const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;
    auto callback = [this] (const FloatArray::SharedPtr msg) -> void   // this is lambda function, and "[this]" gives access to all class members of FirstOrderLowPassFilter
        {
            if (msg->joint_state.size()==joint_names_.size()){
                applicationCommand_.clear();
                for(auto cmd: msg->joint_state){            //shayan defined this msg in robot_interfaces plkg
                    applicationCommand_.push_back(cmd);
                }
            }
        };
    joints_command_subscriber_ = get_node()->create_subscription<FloatArray>("/joints_command",10,callback); //“Whenever a message of type FloatArray arrives on /joints_command, call this function.”
    return CallbackReturn::SUCCESS;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
controller_interface::InterfaceConfiguration FirstOrderLowPassFilter::command_interface_configuration() const  // it tells the controller manager: “Which hardware command interfaces do I want to write to?”
{
    controller_interface::InterfaceConfiguration config;
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;
    config.names.reserve(joint_names_.size());
    for (auto joint_name : joint_names_)
    {
        config.names.push_back(joint_name+"/"+ interface_name_);
    }
    return config;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
controller_interface::InterfaceConfiguration FirstOrderLowPassFilter::state_interface_configuration() const 
{
    controller_interface::InterfaceConfiguration config;
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;
    config.names.reserve(joint_names_.size());
    for (auto joint_name : joint_names_)
    {
        config.names.push_back(joint_name+"/"+ interface_name_);
    }
    return config;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
controller_interface::CallbackReturn FirstOrderLowPassFilter::on_activate(const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;
    applicationCommand_.clear();

    for (int i=0; i < (int)joint_names_.size();i++)
    {   
        applicationCommand_.push_back(state_interfaces_[i].get_optional().value());    //state_interfaces_ is function memebr of ROS2 parent class controller_interface::ControllerInterface
    }
        
    return CallbackReturn::SUCCESS;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
controller_interface::return_type FirstOrderLowPassFilter::update(const rclcpp::Time & time, const rclcpp::Duration & period)
{
    (void)time;
    (void)period;
    for(int i=0; i < (int)joint_names_.size(); i++)
    {
        double state       = state_interfaces_[i].get_optional().value();  //state_interfaces_ is function memebr of ROS2 parent class controller_interface::ControllerInterface
        double cmd         = applicationCommand_[i];
        double control_law = cmd*coefficient_ + state*(1-coefficient_);    
        (void)command_interfaces_[i].set_value(control_law);               //command_interfaces_ is function memebr of ROS2 parent class controller_interface::ControllerInterface
    }
    return controller_interface::return_type::OK;

}


} //end of namespace

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(controller_algo::FirstOrderLowPassFilter, controller_interface::ControllerInterface)