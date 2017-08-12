/** \file
	\brief Provide motion models which merge multi motion estimations.
	\author NanJiang Robot Inc.
	\date 2016-02-26
	\version 0.0.1
*/
#include <common/point.h>
#include <Eigen/Dense>


//����  x1,Q1:�²�1�ľ�ֵ�Ͳ�ȷ���ȣ���� x2,Q2:�²�2�ľ�ֵ�Ͳ�ȷ���ȣ����  
//���  x_out,Q_out: �ںϺ�ľ�ֵ�Ͳ�ȷ���ȣ����
void merge(double x1,double Q1,double x2,double Q2,double& x_out,double& Q_out);

void merge(Eigen::Vector3d x1,Eigen::Matrix3d C1,Eigen::Vector3d x2,Eigen::Matrix3d C2,Eigen::Vector3d& x_out,Eigen::Matrix3d& C_out);

void merge(NJRobot::OrientedPoint x1,NJRobot::OrientedPoint C1,NJRobot::OrientedPoint x2,NJRobot::OrientedPoint C2,NJRobot::OrientedPoint& x_out,NJRobot::OrientedPoint& C_out);
