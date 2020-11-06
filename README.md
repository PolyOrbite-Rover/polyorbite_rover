# ROS_control

Le projet de contrôle du Rover de Polyorbite est en cours de développement. Le modèle du robot se trouve sous urdf/robot1.urdf.xacro.

Pour lancer le projet, assurez-vous d'avoir les librairies complètes ROS Melodic d'installées, ainsi que Gazebo. Il faut aussi avoir clôné le répetoire dans catkin\_ws et y avoir lancé la commande catkin\_make
# Visualisation RVIZ
Pour voir le modèle avec RVIZ, soyez dans le répertoire catkin\_ws/src/polyorbite_rover et lancez la commande:
```bash
roslaunch polyorbite_rover display.launch model:=urdf/rover1.urdf.xacro
```

#Lancement de gazebo avec le module de contrôle differentiel
Pour voir le modèle avec RVIZ, soyez dans le répertoire catkin\_ws/src/polyorbite_rover et lancez la commande:

```bash
roslaunch polyorbite_rover control.launch model:=urdf/rover1.urdf.xacro
```

Vous pourrez alors y contrôler le robot avec l'interface graphique, ou y publish un message
```bash
rostopic pub /rover_diff_drive_controller/cmd_vel geometry_msgs/Twist -r 8 '[Vitesse_longitudinale, 0.0, 0.0]' '[Vitesse_laterale, 0.0, 0.0]'
```
Avec les valeurs

