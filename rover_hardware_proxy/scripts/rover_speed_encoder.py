#!/usr/bin/env python

import numpy as np
import time
import threading
from threading import Thread
import rospy
from std_msgs.msg import Float32MultiArray

import RPi.GPIO as GPIO

VELOCITY_PINS = {
    "front_left_A"    : 29,
    "front_left_B"    : 23
}

class EncodingPublisher(Thread):
    def __init__(self, topic_name, publisher_queue_size = 10, publish_interval = 2, time_history = [], forward = []):
        self.time_history = time_history
        self.forward = forward
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
    if (A_UP == 1) and (B_UP == 0):
        FORWARD = 1
    elif (A_UP == 0) and (B_UP == 1):
        FORWARD = 0
    elif (A_UP == 1) and (B_UP == 1):
        FORWARD = 0
    elif (A_UP == 0) and (B_UP == 0):
        FORWARD = 1

    print("Is forward: %s" % FORWARD)

    return FORWARD

def main():
    initialize_gpio()

    rospy.init_node('rover_velocity_encoder', anonymous=True)

    # Publisher to publish the encoder velocity values
    encodingHandler = EncodingPublisher('/velocity')

    # Length of the wait time before calculating velocity
    LENGTH_TIME_ARRAY = 10

    ## Loop to calculate the velocity every time a time measure is made
    # Variable to identify if the signal is on or off and to determine the direction
    STATE_A = 0
    STATE_B = 0
    A_UP = 0
    B_UP = 0

    while not rospy.is_shutdown():
        #print("Channel A: %s, channel B: %s" % (GPIO.input(VELOCITY_PINS["front_left_A"]), GPIO.input(VELOCITY_PINS["front_left_B"])))

        # To reset the states when the signal turn off
        if GPIO.input(VELOCITY_PINS["front_left_A"]) and (STATE_A == 1):
            STATE_A = 0
            A_UP = 0
        elif GPIO.input(VELOCITY_PINS["front_left_B"]) and (STATE_B == 1):
            STATE_B = 0
            B_UP = 0

        # Loop for recording time stamps when a magnet is detected
        if (GPIO.input(VELOCITY_PINS["front_left_A"]) == False) and (STATE_A == 0):
            STATE_A = 1
            A_UP = 1
            FORWARD = checkDirection(A_UP, B_UP)
            encodingHandler.time_history.append(time.time())
            if FORWARD:
                encodingHandler.forward.append(1)
            else:
                encodingHandler.forward.append(-1)
        elif (GPIO.input(VELOCITY_PINS["front_left_B"]) == False) and (STATE_B == 0):
            STATE_B = 1
            B_UP = 1

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
