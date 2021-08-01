#!/usr/bin/env python

import numpy as np
import time
import threading
from threading import Thread
import rospy
from std_msgs.msg import Float32MultiArray

import RPi.GPIO as GPIO

VELOCITY_PINS = {
    "front_left": {
        "A": 29,
        "B": 23
    },
    "middle_left": {
        "A": 24,
        "B": 22
    },
    "rear_left": {
        "A": 21,
        "B": 19
    },
    "front_right": {
        "_A": 18,
        "_B": 16
    },
    "middle_right": {
        "A": 15,
        "B": 13
    },
    "rear_right": {
        "A": 11,
        "B": 12
    }
}

def checkDirection(A_UP, B_UP):
    FORWARD = 0
    # Loop to determine the direction of rotation
    if (A_UP == 1) and (B_UP == 0):
        FORWARD = 1
    elif (A_UP == 0) and (B_UP == 1):
        FORWARD = 0
    elif (A_UP == 1) and (B_UP == 1):
        FORWARD = 0
    elif (A_UP == 0) and (B_UP == 0):
        FORWARD = 1

    return FORWARD

class EncodingPublisher(Thread):
    def __init__(self, topic_name, publisher_queue_size = 10, publish_interval = 2, time_history = [], forward = []):
        self.time_history = time_history
        self.forward = forward

        self.STATE_A = 0
        self.STATE_B = 0
        self.A_UP = 0
        self.B_UP = 0

        self.publish_interval = publish_interval

        self.publisher = rospy.Publisher(
            topic_name,
            Float32MultiArray,
            queue_size=publisher_queue_size
        )

        self.schedule_next_publication()

    def schedule_next_publication(self):
        timer = threading.Timer(self.publish_interval, self.publish_velocity)
        timer.daemon = True
        timer.start()
    
    def publish_velocity(self):
        velocity_message = Float32MultiArray()

        derivative = []
        if len(self.time_history) <= 1:
            velocity_message.data.append(0)
        else:
            for i in range(len(self.time_history)-1):
                delta_time_sec = self.time_history[i+1] - self.time_history[i]
                if delta_time_sec == 0:
                    continue
                derivative.append(self.forward[i+1] * 2 * (np.pi / 180) / delta_time_sec)
            
            velocity_message.data.append(len(derivative) == 0 if 0 else sum(derivative) / len(derivative))

        # Publish the velocity array into the topic
        self.publisher.publish(velocity_message)
        self.time_history = []
        self.forward = []

        print(velocity_message.data)

        self.schedule_next_publication()

    def update(front_leftpin_A, pin_B):
        # To reset the states when the signal turn off
        if GPIO.input(pin_A) and (self.STATE_A == 1):
            self.STATE_A = 0
            self.A_UP = 0
        elif GPIO.input(pin_B) and (self.STATE_B == 1):
            self.STATE_B = 0
            self.B_UP = 0

        # Loop for recording time stamps when a magnet is detected
        if (GPIO.input(pin_A) == False) and (self.STATE_A == 0):
            self.STATE_A = 1
            self.A_UP = 1
            FORWARD = checkDirection(self.A_UP, self.B_UP)
            encodingHandler.time_history.append(time.time())
            if FORWARD:
                encodingHandler.forward.append(1)
            else:
                encodingHandler.forward.append(-1)
        elif (GPIO.input(pin_B) == False) and (self.STATE_B == 0):
            self.STATE_B = 1
    rospy.init_node('rover_velocity_encod


def initialize_gpio():
    """
    Adapt to new board inputs!
    """
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    for pin in VELOCITY_PINS.values():
        GPIO.setup(pin, GPIO.IN)

def main():
    initialize_gpio()

    rospy.init_node('rover_velocity_encoder', anonymous=True)

    # Publisher to publish the encoder velocity values
    encodingHandler = EncodingPublisher('/velocity')
    encodingHandlers = {
        "front_left": EncodingPublisher('/velocity'),
        "middle_left": EncodingPublisher('/velocity'),
        "rear_left": EncodingPublisher('/velocity'),
        "front_right": EncodingPublisher('/velocity'),
        "middle_right": EncodingPublisher('/velocity'),
        "rear_right": EncodingPublisher('/velocity')
    }

    while not rospy.is_shutdown():
        #print("Channel A: %s, channel B: %s" % (GPIO.input(VELOCITY_PINS["front_left_A"]), GPIO.input(VELOCITY_PINS["front_left_B"])))
        encodingHandlers['front_left'].update(
            VELOCITY_PINS['front_left']['A'],
            VELOCITY_PINS['front_left']['B']
        )

        encodingHandlers['middle_left'].update(
            VELOCITY_PINS['middle_left']['A'],
            VELOCITY_PINS['middle_left']['B']
        )

        encodingHandlers['rear_left'].update(
            VELOCITY_PINS['rear_left']['A'],
            VELOCITY_PINS['rear_left']['B']
        )

        encodingHandlers['front_right'].update(
            VELOCITY_PINS['front_right']['A'],
            VELOCITY_PINS['front_right']['B']
        )

        encodingHandlers['middle_right'].update(
            VELOCITY_PINS['middle_right']['A'],
            VELOCITY_PINS['middle_right']['B']
        )

        encodingHandlers['rear_right'].update(
            VELOCITY_PINS['rear_right']['A'],
            VELOCITY_PINS['rear_right']['B']
        )
        

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
