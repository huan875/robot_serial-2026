from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    config_dir = os.path.join(
        get_package_share_directory('robot_serial'),
        'config'
    )
    config_file = os.path.join(config_dir, 'serial_config.yaml')

    return LaunchDescription([
        Node(
            package='robot_serial',
            executable='serial_node_exe',
            name='serial_node',
            output='screen',
            parameters=[config_file]
        )
    ])