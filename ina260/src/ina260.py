import rospy
from ina260.msg import ina260info







pub = rospy.Publisher('ina260', ina260info, queue_size=10)
rospy.init_node('ina260_node', anonymous=True)
rate = rospy.Rate(10) #10Hz


while not rospy.is_shutdown():
    hello_str = "Hello world %s" % rospy.get_time()
    rospy.loginfo(hello_str)
    pub.publish(hello_str)
    rate.sleep()






