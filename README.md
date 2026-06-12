# Two-arm project with real hardware

Two Dynamixel XL330 series used

all controllers from here:
https://github.com/ros-controls/ros2_controllers/tree/jazzy

Build custom interface:
https://roboticsbackend.com/ros2-create-custom-message/

Common Interfaces:
https://github.com/ros2/common_interfaces/tree/jazzy

Ros2 msg type:
https://docs.ros.org/en/foxy/Concepts/About-ROS-Interfaces.html


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Three main packages are required:
1- description  
2- bring-up  
3- HW interface  


robot_description/
├── launch/
├── urdf/
├── rviz/
├── package.xml
└── CMakeLists.txt


two_arm_robot_bringup/
├── launch/
├── config/
├── package.xml
└── CMakeLists.txt


two_arm_robot_hardware/
├── include/
│   └── two_arm_robot_hardware/
│       ├── two_arm_hardware_interface.hpp
│       └── xl330_driver.hpp
├── src/
│   ├── two_arm_hardware_interface.cpp
├── package.xml
├── plugin.xml
└── CMakeLists.txt


%% Step 1 %% Preparing robot description package in URDF package:
totally 4 files:
1- common_property.xacro  
2- robot.xacro  
3- control.xacro  
4- urdf.xacro  


1- clean up main robot.xacro file (arm.xacro) and test it with rviz  

ros2 launch my_robot_description display.launch.xml  


2- you will have a common_property.xacro file  

3- add the robot.xacro and common_property.xacro files to urdf.xacro file. Later, you will add control.xacro file as well  

4- write control.xacro file and add it to urdf.xacro  


points:
consistent joints name in control.xacro and robot.xacro  


%% Step 2 %% preparing bring-up package it includes controller.yaml and config/launch.xml  


1- write controller.yaml file:
a- select the controller from:
https://github.com/ros-controls/ros2_controllers/tree/jazzy  

b- from plugin.xml file, select controller name, and from parameters.yaml file, select required parameters  

Then update controller.yaml file  


2- update config/launch.xml file with yaml file name just built in last step  
add controllers  
check names for consistency  


3- after colcon build:

ros2 launch my_robot_bringup my_robot.launch.xml  
ros2 run rqt_graph rqt_graph  


4- find topic sending commands to controller:

ros2 topic list  
ros2 topic info /arm_joint_controller/commands  
ros2 interface show std_msgs/msg/Float64MultiArray  
ros2 topic pub -1 /arm_joint_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.4, 0.3]}"  


%% Step 3 %% preparing HW hpp and cpp  

in includes HW.hpp, HW_interface.hpp, HW_interface.cpp  


0- update package name (e.g twoArm_robot_hardware) in package.xml and CMakeLists.txt file  

1- no need to update HW.hpp  

2- in hpp file:  
a- update based on folder name  
b- if namespace/class changed, reflect in cpp  
c- update actuator ID  


3- in cpp file:  
a- depending on controller type, use position or velocity  
b- set/get joint names must match xacro + bringup + yaml  

in cpp:
set_state("arm_joint1/position", Pos_1);

in yaml:
arm_joint_controller:
  ros__parameters:
    joints: ["arm_joint1","arm_joint2"]

in arm.xacro:
<joint name="arm_joint1" type="revolute">


c- CMakeLists.txt add_library section must be updated  


4- at end of cpp file export plugin:

PLUGINLIB_EXPORT_CLASS(two_arm_hardware::TwoArmHardwareInterface, hardware_interface::SystemInterface)


then update plugin.xml:

a- first line must match project name in CMakeLists.txt  
b- must match package name  

CMakeLists.txt:
pluginlib_export_plugin_description_file(...)

arm.ros2_control.xacro must also include plugin  


5- connect hardware (otherwise on_init fails)

a- colcon build  
b- source install/setup.bash  
c- ros2 launch my_robot_bringup my_robot.launch.xml  


6- USB WSL setup:

usbipd list  
usbipd attach --wsl --busid 1-12  
ls /dev/tty*  


7- send command:

ros2 topic pub -1 /arm_joint_controller/commands std_msgs/msg/Float64MultiArray "{data:[0.0,0.0]}"  


8- checks:

ros2 control list_controllers  
ros2 control list_hardware_interfaces  
ros2 control list_hardware_components  
