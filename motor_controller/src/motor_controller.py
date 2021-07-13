#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist

import RPi.GPIO as GPIO

FRONT_LEFT_DIRECTION_PIN   = 40
FRONT_RIGHT_DIRECTION_PIN  = 38
MIDDLE_LEFT_DIRECTION_PIN  = 36
MIDDLE_RIGHT_DIRECTION_PIN = 37
REAR_LEFT_DIRECTION_PIN    = 35
REAR_RIGHT_DIRECTION_PIN   = 33

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

def setDirections(left, right):
    print("Directions: " + str(left) + ", " + str(right))
    GPIO.output(FRONT_LEFT_DIRECTION_PIN, left)
    GPIO.output(MIDDLE_LEFT_DIRECTION_PIN, left)
    GPIO.output(REAR_LEFT_DIRECTION_PIN, left)

    GPIO.output(FRONT_RIGHT_DIRECTION_PIN, right)
    GPIO.output(MIDDLE_RIGHT_DIRECTION_PIN, right)
    GPIO.output(REAR_RIGHT_DIRECTION_PIN, right)

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
    
    rospy.Subscriber('cmd_vel', Twist, velocityCommandReceived)

    rospy.spin()

if __name__ == '__main__':
    try:
        start()
    except rospy.ROSInterruptException:
        pass