# New_ROS_GUI
2023.5.6，新的，基本完成了控制等功能
该软件目前实现的功能：
图像订阅，
运动控制，摄像头云台控制（按钮控制，遥感控制），
电池显示，
各种地图图层显示，
单点导航实现，
设置返航点，实现返航。
该软件是为在校项目机器人专门开发的软件
使用时需注意：
启动机器人的功能：
roslaunch yzz_slam slam.launch slam_methods:=gmapping
roslaunch robotlaunch robot.launch
或者后面将机器人雷达导航添加到robot.launch中
