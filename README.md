# ROS_control


Le projet de contrôle du Rover de Polyorbite est en cours de développement. Le modèle du robot se trouve sous urdf/robot1.urdf.xacro.

Pour lancer le projet, assurez-vous d'avoir les librairies complètes ROS Melodic d'installées, ainsi que Gazebo. Il faut aussi avoir clôné le répetoire dans catkin\_ws et y avoir lancé la commande catkin\_make
# Visualisation RVIZ
Pour voir le modèle avec RVIZ, soyez dans le répertoire catkin\_ws/src/polyorbite_rover et lancez la commande:
```bash
roslaunch polyorbite_rover display.launch model:=urdf/rover1.urdf.xacro
```

# Lancement de gazebo avec le module de contrôle differentiel
Pour voir le modèle avec RVIZ, soyez dans le répertoire catkin\_ws/src/polyorbite_rover et lancez la commande:

```bash
roslaunch polyorbite_rover control.launch model:=urdf/rover1.urdf.xacro
```

Vous pourrez alors y contrôler le robot avec l'interface graphique, ou y publish un message
```bash
rostopic pub /rover_diff_drive_controller/cmd_vel geometry_msgs/Twist -r 8 '[Vitesse_longitudinale, 0.0, 0.0]' '[Vitesse_laterale, 0.0, 0.0]'
```
Avec les valeurs approprié dans les matrices.

# Lancement du contrôle autonome

Il faut s'assurer d'avoir le système robot_localization fonctionnel, il n'est pas installé de base.

Pour activer le module de contrôle autonome, il faut lancer le fichier launch:
```bash
roslaunch polyorbite_rover move_base.launch
```

Ceci prends des ressources assez intenses sur l'ordinateur. L'envoi d'une position à laquelle se diriger sera avec la commande:
```bash
rostopic pub /planner_goal geometry_msgs/PoseStamped '{header: {stamp: now, frame_id: "map"}, pose: {position: {x: 5.0, y: -2.0, z: 0.0}, orientation: {w: 1.0}}}'
```

# Autre

La liste de commandes utiles au développement se trouve sous commandes.txt, on y trouve entre autre des commandes d'analyse de système, d'enregistrement de données et de déboguage.
