#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Int32MultiArray

import RPi.GPIO as GPIO

FRONT_LEFT_DIRECTION_PIN   = 40
FRONT_RIGHT_DIRECTION_PIN  = 38
MIDDLE_LEFT_DIRECTION_PIN  = 36
MIDDLE_RIGHT_DIRECTION_PIN = 37
REAR_LEFT_DIRECTION_PIN    = 35
REAR_RIGHT_DIRECTION_PIN   = 33

MAX_PWM = 50000 # 65535

def velocityCommandReceived(message):
    wantedLinearVelocity = message.linear.x
    wantedAngularVelocity = message.angular.z

    if wantedLinearVelocity != 0:
        steeringDistribution = 1 - ((wantedAngularVelocity + 1) / 2)
        
        leftVelocity = steeringDistribution * wantedLinearVelocity
        rightVelocity = (1 - steeringDistribution) * wantedLinearVelocity

        leftDirection = leftVelocity >= 0
        rightDirection = rightVelocity >= 0

        setVelocities(abs(leftVelocity), abs(rightVelocity))
        setDirections(leftDirection, rightDirection)

    elif wantedAngularVelocity != 0:
        leftDirection = wantedAngularVelocity <= 0
        rightDirection = wantedAngularVelocity >= 0

        setVelocities(abs(wantedAngularVelocity), abs(wantedAngularVelocity))
        setDirections(leftDirection, rightDirection)
    
    else:
        setVelocities(0, 0)
        setDirections(True, True)

def setVelocities(left, right):
    print("Velocities: " + str(left) + ", " + str(right))
    message = Int32MultiArray()
    message.data = [
        getPwmFromVelocity(left),
        getPwmFromVelocity(right),
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1
    ]

    pwmPublisher.publish(message)
    

def setDirections(left, right):
    print("Directions: " + str(left) + ", " + str(right))
    GPIO.output(FRONT_LEFT_DIRECTION_PIN, left)
    GPIO.output(MIDDLE_LEFT_DIRECTION_PIN, left)
    GPIO.output(REAR_LEFT_DIRECTION_PIN, left)

    GPIO.output(FRONT_RIGHT_DIRECTION_PIN, right)
    GPIO.output(MIDDLE_RIGHT_DIRECTION_PIN, right)
    GPIO.output(REAR_RIGHT_DIRECTION_PIN, right)

def getPwmFromVelocity(velocity):
    return velocity * MAX_PWM

def init_gpio():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(FRONT_LEFT_DIRECTION_PIN, GPIO.OUT)
    GPIO.setup(FRONT_RIGHT_DIRECTION_PIN, GPIO.OUT)
    GPIO.setup(MIDDLE_LEFT_DIRECTION_PIN, GPIO.OUT)
    GPIO.setup(MIDDLE_RIGHT_DIRECTION_PIN, GPIO.OUT)
    GPIO.setup(REAR_LEFT_DIRECTION_PIN, GPIO.OUT)
    GPIO.setup(REAR_RIGHT_DIRECTION_PIN, GPIO.OUT)

def start():
    init_gpio()

    rospy.init_node('motor_controller', anonymous=True)
    
    rospy.Subscriber('/rover_diff_drive_controller/cmd_vel', Twist, velocityCommandReceived)

    global pwmPublisher
    pwmPublisher = rospy.Publisher('command', Int32MultiArray, queue_size = 10)

    rospy.spin()

if __name__ == '__main__':
    try:
        start()
    except rospy.ROSInterruptException:
        pass