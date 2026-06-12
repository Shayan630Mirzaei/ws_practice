# Two-Arm ROS2 Control Project (Real Hardware)

This project implements a **two-arm robotic system** using **Dynamixel XL330 series motors** and `ros2_control`.

---

## 📚 References

- Controllers (ROS2 Jazzy):
  :contentReference[oaicite:0]{index=0}

- Custom Hardware Interface Guide:
  :contentReference[oaicite:1]{index=1}

- Common Interfaces:
  :contentReference[oaicite:2]{index=2}

- ROS2 Interface Concepts:
  :contentReference[oaicite:3]{index=3}

---

## 🧩 Project Structure

Three main packages are required:

### 1. Robot Description Package
```
robot_description/
├── launch/
├── urdf/
├── rviz/
├── package.xml
└── CMakeLists.txt
```

### 2. Bring-up Package
```
two_arm_robot_bringup/
├── launch/
├── config/
├── package.xml
└── CMakeLists.txt
```

### 3. Hardware Interface Package
```
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
```

---

## ⚙️ Step 1 — Robot Description (URDF/XACRO)

1. `common_property.xacro`
2. `robot.xacro`
3. `control.xacro`
4. `urdf.xacro`

```bash
ros2 launch my_robot_description display.launch.xml
```

---

## 🚀 Step 2 — Bring-up Package

### Build & Run
```bash
colcon build
source install/setup.bash
ros2 launch my_robot_bringup my_robot.launch.xml
```

### RQT Graph
```bash
ros2 run rqt_graph rqt_graph
```

### Controller Test
```bash
ros2 topic pub -1 /arm_joint_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.4, 0.3]}"
```

---

## 🔧 Step 3 — Hardware Interface

### Key Files
- two_arm_hardware_interface.hpp
- two_arm_hardware_interface.cpp
- xl330_driver.hpp

---

### Important Rules

#### Joint consistency
```cpp
set_state("arm_joint1/position", Pos_1);
```

```yaml
joints: ["arm_joint1", "arm_joint2"]
```

```xml
<joint name="arm_joint1" type="revolute">
```

---

### Plugin Export
```cpp
PLUGINLIB_EXPORT_CLASS(
  two_arm_hardware::TwoArmHardwareInterface,
  hardware_interface::SystemInterface
)
```

---

### USB (WSL)
```bash
usbipd list
usbipd attach --wsl --busid 1-12
ls /dev/tty*
```

---

### Debug
```bash
ros2 control list_controllers
ros2 control list_hardware_interfaces
ros2 control list_hardware_components
```

---

## ⚠️ Key Rule

All names must match across:
- URDF
- YAML
- C++ hardware interface
```
