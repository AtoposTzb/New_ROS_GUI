/**
 * @file /include/rosqt_gui/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef rosqt_gui_QNODE_HPP_
#define rosqt_gui_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>//sudu
#include <map>
#include <nav_msgs/Odometry.h>//里程计话题
//#include <std_msgs/Float32.h>//电池电压
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>//存放图像编码格式
#include <sensor_msgs/CompressedImage.h>//
#include <QImage>
#include <geometry_msgs/PoseWithCovarianceStamped.h>//位姿的消息类型
#include <geometry_msgs/PoseStamped.h>//导航目标点的消息类型

#include <actionlib/client/simple_action_client.h>
#include <actionlib/server/simple_action_server.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <sensor_msgs/BatteryState.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Float64.h>
#include <tf/transform_listener.h>
#include <QDebug>

#include <QImage>
#include <QLabel>
#include <QSettings>
#include <QStringListModel>
#include <QtConcurrent/QtConcurrent>
#include "../include/rosqt_gui/data_format.h"

//自定义类型
#include <yzz_msgs/Battery.h>//这辆小车的电压为yzz_msgs类型
#include <yzz_msgs/SetHolder.h>
#include <yzz_msgs/GetHolder.h>
#include <yzz_msgs/Imu.h>
#include <yzz_msgs/Lifts.h>
#include <yzz_msgs/PID.h>
#include <yzz_msgs/Sensor.h>//温湿度
#include <yzz_msgs/Velocities.h>
/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace rosqt_gui {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	bool init(const std::string &master_url, const std::string &host_url);
    void set_cmd_vel(char k,float linear,float angular);//一个公共函数去连接键盘控制和速度的功能
    void move_base(char k, float speed_linear, float speed_trun);//遥感控制
    void set_PTZ_vel(char k, float pan_speed, float tilt_speed);//云台控制
    void sub_image(QString topic_name);
    void set_goal(double x, double y, double z);
	void run();

	/*********************
	** Logging
	**********************/
	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };

	QStringListModel* loggingModel() { return &logging_model; }
	void log( const LogLevel &level, const std::string &msg);

Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown();
    void speed_vel(float,float);//因为这是两个类,ui界面是在mianw访问，所以这里需要我们创建自定义信号，把当前的X,Y轴线速度通过信号的方式发送到mainw类中
    void battery_vel(_Float32,char);
    void image_val(QImage);
    void position(double x,double y,double z);

private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;
    ros::Publisher cmd_vel_pub;//速度控制发布者
    ros::Publisher New_PTZ_pub;//云台PTZ
    ros::Subscriber Now_PTZ_sub;//云台订阅者
    ros::Publisher set_pose_pub;
    ros::Publisher goal_pub;//发布导航目标点的话题发布者S

    QStringListModel logging_model;
    ros::Subscriber chatter_sub;//创建一个订阅者
    ros::Subscriber odom_sub;//里程计话题订阅者
    ros::Subscriber battery_sub;//电池电压
    ros::Subscriber set_pose_sub;//位姿的

    image_transport::Subscriber image_sub;
    yzz_msgs::GetHolder NowPTZ;

    void chatter_callback(const std_msgs::String &msg);//shengminghuidiaohanshu
    void odom_callback(const nav_msgs::Odometry &msg);
    void battery_callback(const yzz_msgs::Battery &msg);
    void image_callback(const sensor_msgs::ImageConstPtr &msg);
    void amcl_pose_callback(const geometry_msgs::PoseWithCovarianceStamped &msg);
    void GetHolderCallback(yzz_msgs::GetHolder msg);
    QImage Mat2QImage(cv::Mat const& src);
};

}  // namespace rosqt_gui

#endif /* rosqt_gui_QNODE_HPP_ */
