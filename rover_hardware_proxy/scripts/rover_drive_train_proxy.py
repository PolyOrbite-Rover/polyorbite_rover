#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Int32MultiArray

import RPi.GPIO as GPIO

DIRECTION_PINS = {
    "front_left"    : 40,
    "middle_left"   : 38,
    "rear_left"     : 36,
    "front_right"   : 37,
    "middle_right"  : 35,
    "rear_right"    : 33
}

MAX_PWM = 50000
MAX_VELOCITY_COMMAND = 10

def velocityCommandReceived(message):
    velocities = []
    directions = []
    for i in range(len(message.data)):
        velocity = message.data[i]
        directions.append(velocity >= 0)
        velocities.append(abs(velocity))
    
    changeWheelVelocities(velocities)
    changeWheelDirections(directions)

def changeWheelVelocities(velocities):
    message = Int32MultiArray()
    message.data = []
    for velocity in velocities:
        message.data.append(velocity / MAX_VELOCITY_COMMAND * MAX_PWM)
    
    for i in range(10):
        message.data.append(-1)

    pwmPublisher.publish(message)

def changeWheelDirections(directions):
    for i in range(len(directions)):
        pin = DIRECTION_PINS.values()[i]
        direction = directions[i]
        GPIO.output(pin, direction)

def initialize_gpio():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    for pin in DIRECTION_PINS.values():
        GPIO.setup(pin, GPIO.OUT)

def start():
    initialize_gpio()
    
    rospy.init_node('rover_drive_train_proxy', anonymous=True)
    
    rospy.Subscriber('/drive_train_state', Float32MultiArray, velocityCommandReceived)

    global pwmPublisher
    pwmPublisher = rospy.Publisher('command', Int32MultiArray, queue_size = 10)

    rospy.spin()

if __name__ == '__main__':
    try:
        start()
    except rospy.ROSInterruptException:
        pass