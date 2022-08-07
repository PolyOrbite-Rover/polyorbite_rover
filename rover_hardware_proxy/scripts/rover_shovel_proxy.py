#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32
from std_msgs.msg import Int32MultiArray


MAX_PWM = 65535

def shovelCommandReceived(message):
    speed = message.data
    changeServoSpeed(speed)

def changeServoSpeed(speed):
    message = Int32MultiArray()
    message.data = []

    for i in range(6):
        message.data.append(-1)

    message.data.append((speed + 1) / 2 * MAX_PWM)
    
    for i in range(9):
        message.data.append(-1)

    pwmPublisher.publish(message)

def start():    
    rospy.init_node('rover_shovel_proxy', anonymous=True)
    
    rospy.Subscriber('/shovel_state', Float32, shovelCommandReceived)

    global pwmPublisher
    pwmPublisher = rospy.Publisher('command', Int32MultiArray, queue_size = 10)

    rospy.spin()

if __name__ == '__main__':
    try:
        start()
    except rospy.ROSInterruptException:
        pass
