This is a two-arm project with real hardware.<br>
Two Dynamixel XL330 series used
<br>
all controllers from here:  https://github.com/ros-controls/ros2_controllers/tree/jazzy
<br>
Build custom interface: https://roboticsbackend.com/ros2-create-custom-message/
<br>
Common Interfaces: https://github.com/ros2/common_interfaces/tree/jazzy
<br>
Ros2 msg type: https://docs.ros.org/en/foxy/Concepts/About-ROS-Interfaces.html
<br>
%%%%%%%%%%%

%%%%%%%%%%%<br>
Three main packages are required: 1- description, 2- bring-up, 3-HW interface

robot_description/<br>
├── launch/<br>
├── urdf/<br>
├── rviz/<br>
├── package.xml<br>
└── CMakeLists.txt<br>
<br>
two_arm_robot_bringup/<br>
├── launch/<br>
├── config/<br>
├── package.xml<br>
└── CMakeLists.txt<br>
<br>
two_arm_robot_hardware/<br>
├── include/<br>
│   └── two_arm_robot_hardware/<br>
│       ├── two_arm_hardware_interface.hpp<br>
│       └── xl330_driver.hpp<br>
├── src/<br>
│   ├── two_arm_hardware_interface.cpp<br>
├── package.xml<br>
├── plugin.xml<br>
└── CMakeLists.txt<br>


%% Step 1%% 
Preparing robot description package
in URDF package: totally 4 files:1-common_prperty.xacro, 2-robot.xacro, 3-control.xacro, 4-urdf.xacro

1- clean up main robot.xacro file (arm.xacro) file and test it with rviz

ros2 launch my_robot_description display.launch.xml

2- you will have a common_property.xacro file 
3- add the robot.xacro and common_property.xacro files to urdf.xacro file. Later, you will add control.xacro file as well
4- write control.xacro file and add it to urdf.xacro
 
points: consistent joints name in control.xacro and robot.xacro




%% Step 2 %% 
preparing bring-up package
it includes controller.yaml and config/launch.xml

1- write controller.yaml file: 
                          a-select the controller from this website https://github.com/ros-controls/ros2_controllers/tree/jazzy
                          b- From plug_in.xml file, select controller name, and from src/paramters.yaml file, select requred parameters. Then update controller.yaml file
2- update config/launch.xml file with ymal file name just built in last step, added controllers, check names for consistency

3- after colcon build, run
                         ros2 launch my_robot_bringup my_robot.launch.xml
                         ros2 run rqt_graph rqt_graph
4- find the topic sends commands to controller, and find it types info, and try an example
                         ros2 topic list
                         ros2 topic info /arm_joint_controller/commands
                         ros2 interface show std_msgs/msg/Float64MultiArray
                         ros2 topic pub -1 /arm_joint_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.4, 0.3]}" 



<br>
%%%%%%%%%%%%%%%%%%<br>
%% Step 3 %%%%%%%% <br>
%%%%%%%%%%%%%%%%%%<br>
preparing HW hpp and cpp<br>
in includes HW.hpp, HW_interface.hpp, and HW_interface.cpp<br>
<br>
1. update package name (e.g twoArm_robot_hardware) in package.xml and Cmakelist.txt file
<br>
2. no need to update HW.hpp
<br>
3. In hpp file:
<br>
- a) update based on folder name
                <br>
- b) if you update namespace and class name, reflect it in HW_interface.cpp
                <br>
- c) update actuator ID
                <br>
<br>
4. in cpp file:
<br>
- a) depending on controller type, activate, set function should be update based on position or velocit
                y<br>
- b) whenever using set/get function, joints name, or motor names should be consistent with xacro file from discerption package and bring-up package
                <br>

                                                  in cpp:   set_state("arm_joint1/position", Pos_1);
                                                  in yaml:  arm_joint_controller:
                                                                  ros__parameters:
                                                                      joints: ["arm_joint1","arm_joint2"]
                                                 
                                                 in arm.xacro: <joint name="arm_joint1" type="revolute">
                c- Cmakelist.txt file (add_library section) should be updated with cpp file name
                <br>


5. At the end of cpp file, export the cpp class via plugin 
<br>
                    
                     PLUGINLIB_EXPORT_CLASS(two_arm_hardware::TwoArmHardwareInterface, hardware_interface::SystemInterface)

then build the plugin.xml file in HW package, and update other files with this plugin, including:

- a)first line of plugin.xml (<library path="two_arm_robot_hardware">) should match with package name and project name in Cmakelist.txt file (project(two_arm_robot_hardware)). 
- b) Cmakelist.txt file -->> pluginlib_export_plugin_description_file()               
- c) arm.Ros2Control.xacro in description package --->>
- 
<br>

6. Powershell doesn't share usb ports with wsl. Should do it manually. This causes on_onit and on_configure errors

- a) usbipd list
- b) usbipd attach --wsl --busid 1-12
- c) ls /dev/tty*


6. connect hardware (otherwise on_init failure) and run 
- a) colcon build
- b) source install/setup.bash
- c) ros2 launch my_robot_bringup my_robot.launch.xml
<br>
<br>
8. send command:
- ros2 topic list<br>
- ros2 topic info /joints_command<br>
- ros2 interface show robot_interfaces/msg/JointCurrentPose<br>
-  ros2 topic pub -1 /joints_command robot_interfaces/msg/JointCurrentPose "{joint_state:[0.5,0.5]}"
<br>
<br>

8- some checking
          
               a- ros2 control list_controllers
               b- ros2 control list_hardware_interfaces
               c- ros2 control list_hardware_components
