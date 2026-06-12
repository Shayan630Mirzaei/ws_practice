# Two-Arm ROS2 Control Project (Real Hardware)

This project implements a two-arm robotic system using Dynamixel XL330 series motors and ros2_control.

## References

- Controllers (ROS2 Jazzy):
  https://github.com/ros-controls/ros2_controllers/tree/jazzy

- Custom Hardware Interface Guide:
  https://roboticsbackend.com/ros2-create-custom-message/

- Common Interfaces:
  https://github.com/ros2/common_interfaces/tree/jazzy

- ROS2 Interface Concepts:
  https://docs.ros.org/en/foxy/Concepts/About-ROS-Interfaces.html


## Project Structure

### Robot Description Package
robot_description/
├── launch/
├── urdf/
├── rviz/
├── package.xml
└── CMakeLists.txt


### Bring-up Package
two_arm_robot_bringup/
├── launch/
├── config/
├── package.xml
└── CMakeLists.txt


### Hardware Interface Package
two_arm_robot_hardware/
├── include/
│   └── two_arm_robot_hardware/
│       ├── two_arm_hardware_interface.hpp
│       └── xl330_driver.hpp
├── src/
│   └── two_arm_hardware_interface.cpp
├── package.xml
├── plugin.xml
└── CMakeLists.txt


## Step 1 — Robot Description (URDF/XACRO)

1- common_property.xacro  
2- robot.xacro  
3- control.xacro  
4- urdf.xacro  

ros2 launch my_robot_description display.launch.xml  


## Step 2 — Bring-up Package

1- controller.yaml file  
   a- select controller from ros2_controllers (jazzy)  
   https://github.com/ros-controls/ros2_controllers/tree/jazzy  
   
   b- from plugin.xml select controller name  
   c- from parameters.yaml select required parameters  

2- update launch.xml with yaml file name  
   ensure controller names match  

3- build and run  

colcon build  
source install/setup.bash  
ros2 launch my_robot_bringup my_robot.launch.xml  

ros2 run rqt_graph rqt_graph  


4- test controller  

ros2 topic list  
ros2 topic info /arm_joint_controller/commands  
ros2 interface show std_msgs/msg/Float64MultiArray  
ros2 topic pub -1 /arm_joint_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.4, 0.3]}"  


## Step 3 — Hardware Interface

0- update package name in package.xml and CMakeLists.txt  

1- hpp file  
a- update namespace based on folder name  
b- update class name if changed  
c- update actuator IDs  

2- cpp file  
a- set position or velocity based on controller type  
b- ensure joint names match across all files  

set_state("arm_joint1/position", Pos_1);  

yaml:
arm_joint_controller:
  ros__parameters:
    joints: ["arm_joint1", "arm_joint2"]  

urdf:
<joint name="arm_joint1" type="revolute">


3- CMakeLists.txt  
add_library(...) must include cpp file  


4- plugin export  

PLUGINLIB_EXPORT_CLASS(two_arm_hardware::TwoArmHardwareInterface, hardware_interface::SystemInterface)  


5- plugin.xml rules  
- first line must match project name in CMakeLists.txt  
- must match package name  
- must be exported in CMakeLists.txt  


6- build and connect hardware  

colcon build  
source install/setup.bash  
ros2 launch my_robot_bringup my_robot.launch.xml  


7- USB WSL connection  

usbipd list  
usbipd attach --wsl --busid 1-12  
ls /dev/tty*  


8- send command  

ros2 topic pub -1 /arm_joint_controller/commands std_msgs/msg/Float64MultiArray "{data:[0.0,0.0]}"  


9- debug commands  

ros2 control list_controllers  
ros2 control list_hardware_interfaces  
ros2 control list_hardware_components  
```
