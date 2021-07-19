#!/usr/bin/env python2

import rospy
from ina260.msg import ina260info

pub = rospy.Publisher('ina260', ina260info, queue_size=10)
rospy.init_node('ina260_node', anonymous=True)
rate = rospy.Rate(10) #10Hz 

while not rospy.is_shutdown():
    message = ina260info()
    message.Power = 3
    message.Current = 2
    message.Voltage = 1

    pub.publish(message)

    rate.sleep()