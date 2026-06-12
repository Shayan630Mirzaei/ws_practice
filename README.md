# Two-Arm ROS2 Control Project

This is a two-arm project with real hardware.

---

# Step 2: Bring-up Package

This package includes:
- `controller.yaml`
- `config/launch.xml`

---

## 1. Create `controller.yaml`

- Select a controller from:
  https://github.com/ros-controls/ros2_controllers/tree/jazzy

- From `plugin.xml`:
  - Get controller plugin name

- From controller `parameters.yaml`:
  - Extract required parameters
  - Update `controller.yaml`

---

## 2. Update launch file

- Edit `config/launch.xml`
- Add the new YAML file
- Ensure controller names match consistently

---

## 3. Build and run

```bash
colcon build
ros2 launch my_robot_bringup my_robot.launch.xml
