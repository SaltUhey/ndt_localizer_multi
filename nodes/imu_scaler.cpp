#include <ros/ros.h>
#include <sensor_msgs/Imu.h>

class ImuScaler {
public:
  ImuScaler() {
    // IMUデータの購読
    imu_sub = nh.subscribe("/livox/imu", 10, &ImuScaler::imuCallback, this);

    // 変換後のIMUデータを新しいトピックにパブリッシュ
    imu_pub = nh.advertise<sensor_msgs::Imu>("/livox/imu_converted", 10);
  }

  void imuCallback(const sensor_msgs::Imu::ConstPtr& msg) {
    sensor_msgs::Imu converted_msg = *msg;

    converted_msg.header.frame_id = "imu_link";

    // NED -> ENU変換（X軸とY軸を入れ替え、Z軸の符号を反転）
    // double temp_x = converted_msg.linear_acceleration.x;
    // converted_msg.linear_acceleration.x = converted_msg.linear_acceleration.y;
    // converted_msg.linear_acceleration.y = temp_x;
    // converted_msg.linear_acceleration.z *= -1.0;

    // Z軸（垂直方向）の加速度値を[m/s²]に変換
    converted_msg.linear_acceleration.z *= 9.80665;

    // 角速度も同様にX軸とY軸を入れ替え、Z軸を反転
    // temp_x = converted_msg.angular_velocity.x;
    // converted_msg.angular_velocity.x = converted_msg.angular_velocity.y;
    // converted_msg.angular_velocity.y = temp_x;
    // converted_msg.angular_velocity.z *= -1.0;

    // 新しいトピックに変換後のIMUデータをパブリッシュする
    imu_pub.publish(converted_msg);
  }
private:
  ros::NodeHandle nh;
  ros::Subscriber imu_sub;
  ros::Publisher imu_pub;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "imu_scaler");

  ImuScaler imu_scaler;

  ros::spin();
  return 0;
}