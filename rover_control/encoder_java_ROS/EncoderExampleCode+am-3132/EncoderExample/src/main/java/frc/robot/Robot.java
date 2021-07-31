/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

/**
 * This example shows how to use an Encoder in an FRC program.
 *
**/
package encoder_java_ROS;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.Encoder;


public class Robot extends TimedRobot {
  /**
   * This function is run when the robot is first started up and should be used
   * for any initialization code.
   */
  Encoder enc;

  //private static final double cpr = 7/4; //if am-2861a
  private static final double cpr = 360; //if am-3132
  // private static final double cpr = 5; //if am-3314a
  // private static final double cpr = 1024; //if am-3445
  // private static final double cpr = 64; //if am-4027

  private static final double whd = 6; // for 6 inch wheel


  @Override
  public void robotInit() {
    enc = new Encoder(0,1);
    enc.setDistancePerPulse(Math.PI*whd/cpr); //distance per pulse is pi* (wheel diameter / counts per revolution)
  }

  @Override
  public void robotPeriodic()
  {
    double dist = enc.getDistance();
    SmartDashboard.putNumber("Encoder", dist);
  }
}
