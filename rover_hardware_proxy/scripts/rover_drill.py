import RPi.GPIO as GPIO

HEIGHT_PIN = 40
SPIN_PIN = 38

GPIO.setmode(GPIO.BOARD)

GPIO.setup(HEIGHT_PIN, OUTPUT)
GPIO.setup(SPIN_PIN, OUTPUT)


