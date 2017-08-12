/** \file
	\brief Safe speed adjust with laser scan
	\author NanJiang Robot Inc.
	\date 2016-02-26
	\version 0.0.1
*/
#pragma once
#include "abstract_safe_adjust.h"
#include <common/robot_type.h>

namespace NJRobot{

class LaserSafeAdjust:public AbstractSafeAdjust<PointList>
{
public:
	LaserSafeAdjust(void);
	~LaserSafeAdjust(void);

	void safeSpeedAdjust(RobotSpeed& send_speed,const PointList & obs,const RobotSpeed& actual_speed);
	
private:
	void dangerCheck(const PointList & obs,const RobotSpeed& actual_speed); //m_obstacle_points�в��ҿ��Ƿ����ϰ����

	bool			m_has_obs_front;  //��������ǰ���Ƿ����ϰ������У�����м���ʲô�ģ�����������ϰ������m_obs_x������
	bool			m_has_obs_left,m_has_obs_right;   //�������������ϰ���
	Point			m_closest_point,m_obs_point_left,m_obs_point_right;

};

}

