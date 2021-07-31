#!/usr/bin/env python

import numpy as np
import threading
import rospy
from std_msgs.msg import Float32MultiArray

import RPi.GPIO as GPIO

VELOCITY_PINS = {
    "front_left_A"    : 29,
    "front_left_B"    : 32
}

# def encoderMessageReceived():
#     """ Every time a new time stamp is received, the velocity is calculated """
#     time = np.datetime64('now')
#     if GPIO.input(input_pin_num):
#         detected_time = np.datetime64('now')


def initialize_gpio():
    """
    Adapt to new board inputs!
    """
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    for pin in VELOCITY_PINS.values():
        GPIO.setup(pin, GPIO.IN)

def checkDirection(A_UP, B_UP):
    FORWARD = 0
    # Loop to determine the direction of rotation
    if (A_UP == 1) && (B_UP == 0):
        FORWARD = 0
    elif (A_UP == 0) && (B_UP == 1):
        FORWARD = 0
    elif (A_UP == 1) && (B_UP == 1):
        FORWARD = 1
    elif (A_UP == 0) && (B_UP == 0):
        FORWARD = 1

    return FORWARD

def publisher(time, velocity_message, forward):
    derivative = []
    if len(time) <= 1:
        velocity_message.data.append(0)
    else:
        for i in range(len(time)-1):
            delta_time_ns = (time[i+1] - time[i]) / np.timedelta64(1, 'ns')
            derivative.append(forward[i+1] * np.pi / delta_time_ns)
        velocity_message.data.append(sum(derivative) / len(derivative))

    # Publish the velocity array into the topic
    encoder_Publisher.publish(velocity_message)
    print(velocity_message.data)

def main():
    initialize_gpio()

    rospy.init_node('rover_velocity_encoder', anonymous=True)

    # Publisher to publish the encoder velocity values
    encoder_Publisher = rospy.Publisher('/velocity', Float32MultiArray, queue_size=10)

    # Length of the wait time before calculating velocity
    LENGTH_TIME_ARRAY = 10

    ## Loop to calculate the velocity every time a time measure is made
    # Variable to identify if the signal is on or off and to determine the direction
    STATE_A = 0
    STATE_B = 0
    A_UP = 0
    B_UP = 0

    # Average time before calculating velocity despite time array not full
    MAX_TIME_BEFORE_PUBLISH = 15  # seconds

    while not rospy.is_shutdown():
        time = []
        forward = []
        velocity_message = Float32MultiArray()

        # To reset the states whent he signal turn off
        if GPIO.input(29):
            STATE_A = 0
            A_UP = 0
        elif GPIO.input(32):
            STATE_B = 0
            B_UP = 0

        # Loop for recording time stamps when a magnet is detected
        if (GPIO.input(29) == False) && (STATE_A == 0):
            STATE_A = 1
            A_UP = 1
            FORWARD = checkDirection(A_UP, B_UP)
            time.append(np.datetime64('now'))
            if FORWARD:
                forward.append(1)
            else:
                forward.append(-1)
        elif (GPIO.input(32) == False) && (STATE_B == 0):
            STATE_B = 1
            B_UP = 1
            FORWARD = checkDirection(A_UP, B_UP)
            time.append(np.datetime64('now'))
            if FORWARD:
                forward.append(1)
            else:
                forward.append(-1)



        # if len(time) == LENGTH_TIME_ARRAY :
        #     publisher(time, velocity_message)

        # If the MAX_TIME_BEFORE_PUBLISH has been reached, publish anyway
        start_time = threading.Timer(MAX_TIME_BEFORE_PUBLISH, publisher(time, velocity_message, forward))


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
