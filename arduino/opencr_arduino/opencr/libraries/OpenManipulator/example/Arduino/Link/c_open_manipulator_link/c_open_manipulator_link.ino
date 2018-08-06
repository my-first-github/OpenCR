//#include <OpenManipulator.h>
#include <OMManager.h>
#include <OMMath.h>
#include <OMKinematics.h>
#include <OMDebug.h>
//#include "OpenManipulator_Link.h"

#define WORLD     0
#define BASE      1
#define JOINT0    2
#define JOINT1    3
#define JOINT2    4
#define JOINT3    5
#define JOINT4    6
#define JOINT5    7
#define JOINT6    8
#define JOINT7    9
#define JOINT8    10
#define JOINT9    11
#define JOINT10   12
#define SUCTION   13

void myGetPassiveJointAngle(Manipulator* omlink);

Manipulator omlink;

void setup() 
{
  Serial.begin(57600);
  while (!Serial)
    ;
  
  omlink.addWorld(WORLD, BASE);
  omlink.addComponent(BASE, WORLD, JOINT0, MATH::makeVector3(-0.23867882, 0, 0), Matrix3f::Identity(3,3));
  omlink.addComponent(JOINT0, BASE, JOINT1, Vector3f::Zero(), Matrix3f::Identity(3,3), MATH::makeVector3(0,0,1), 1, -1);
  omlink.addComponentChild(JOINT0, JOINT2);
  omlink.addComponentChild(JOINT0, JOINT7);
  omlink.addComponent(JOINT1, JOINT0, JOINT5, MATH::makeVector3(0, 0.022, 0.052), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0), 2, 1);
  omlink.addComponent(JOINT2, JOINT0, JOINT3, MATH::makeVector3(0, -0.022, 0.052), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0), 3, -1);
  omlink.addComponent(JOINT3, JOINT2, JOINT4, MATH::makeVector3(0.050, 0.007, 0), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addComponent(JOINT4, JOINT3, JOINT5, MATH::makeVector3(0.200, 0.006, 0), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addComponent(JOINT5, JOINT1, JOINT6, MATH::makeVector3(0.200, -0.016, 0), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addComponent(JOINT6, JOINT5, SUCTION, MATH::makeVector3(0.200, -0.009, 0), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addComponent(JOINT7, JOINT0, JOINT8, MATH::makeVector3(-0.04531539, 0.006, 0.07313091), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addComponent(JOINT8, JOINT7, JOINT9, MATH::makeVector3(0.200, 0.009, 0), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addComponent(JOINT9, JOINT8, JOINT10, MATH::makeVector3(0.07660444, -0.006, 0), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addComponent(JOINT10, JOINT9, SUCTION, MATH::makeVector3(0.200, -0.006, 0), Matrix3f::Identity(3,3), MATH::makeVector3(0,1,0));
  omlink.addTool(SUCTION, JOINT6, MATH::makeVector3(0.03867882, 0.003, -0.01337315), Matrix3f::Identity(3,3), 4, 1);

  myGetPassiveJointAngle(&omlink);
  //omlink.checkManipulatorSetting();

  omlink.setComponentJointAngle(JOINT0, 0.0);
  omlink.setComponentJointAngle(JOINT1, -M_PI/2 + 0.0*DEG2RAD);
  omlink.setComponentJointAngle(JOINT2, -M_PI + 0.0*DEG2RAD);
  myGetPassiveJointAngle(&omlink);
  KINEMATICS::LINK::forward(&omlink);
  omlink.checkManipulatorSetting();

  VectorXf target_angle(omlink.getDOF());
  Pose target_pose;
  target_pose.position = MATH::makeVector3(0.05, -0.05, 0.0);
  target_pose.orientation = Matrix3f::Identity(3,3);

  target_angle = KINEMATICS::LINK::geometricInverse(&omlink, SUCTION, target_pose);
  
  target_angle = target_angle*RAD2DEG;
  
  USB.println("\nTest Inverse");
  USB.println(" target_pose : ");
  PRINT::VECTOR(target_pose.position);
  USB.println(" target_angle : ");
  PRINT::VECTOR(target_angle);
}

void loop() 
{
//  myGetPassiveJointAngle(omlink);

}

void myGetPassiveJointAngle(Manipulator* omlink)
{
  float joint_angle[3];
  joint_angle[0] = omlink->getComponentJointAngle(JOINT0);
  joint_angle[1] = omlink->getComponentJointAngle(JOINT1);
  joint_angle[2] = omlink->getComponentJointAngle(JOINT2);

  omlink->setComponentJointAngle(JOINT3, -(joint_angle[1]-joint_angle[2]));
  omlink->setComponentJointAngle(JOINT4, -M_PI-(joint_angle[1]-joint_angle[2]));
  omlink->setComponentJointAngle(JOINT5, -M_PI-(joint_angle[1]-joint_angle[2]));
  omlink->setComponentJointAngle(JOINT6, M_PI-joint_angle[2]);
  omlink->setComponentJointAngle(JOINT7, joint_angle[1]);
  omlink->setComponentJointAngle(JOINT8, -(15 * DEG2RAD)-joint_angle[1]);
  omlink->setComponentJointAngle(JOINT9, -(joint_angle[2])-(165 * DEG2RAD));
  omlink->setComponentJointAngle(JOINT10, (270 * DEG2RAD)-joint_angle[2]);
}