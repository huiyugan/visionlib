/** \file
	\brief Motion control with finite state machine
	\author NanJiang Robot Inc.
	\date 2016-02-26
	\version 0.0.1
*/

#ifndef NRF_MOTION_CONTROL_FSM_H
#define NRF_MOTION_CONTROL_FSM_H

#include "abstract_motion_control.h"
#include <common/types.h>
#include "laser_safe_adjust.h"
#include <common/logger.h>

namespace NJRobot
{

class MotionControlFSM : public AbstractMotionControl {
public:
	MotionControlFSM();
	~MotionControlFSM();

	LaserSafeAdjust * getLaserSafeAdjuster(){
		return &m_laser_safe_handler;
	}

protected:
	virtual void computeCurSpeed();
	virtual void safeAdjust();

protected:
	virtual bool doAction(double &vx_best , double &vy_best , double &w_best);
	virtual void checkStateChange();
	bool actionStraight(double &v_best , double &w_best);
	bool actionReach(double &v_best , double &w_best);
	bool actionTurn(double &v_best , double &w_best);
	bool actionStop(double &v_best , double &w_best);
	bool actionNear(double &v_best , double &w_best);
	bool actionSimpleReach(double &v_best , double &w_best);
	bool actionCoarseReach(double &v_best,double &w_best);
	void changeStateTo(int state);

	void smoothSpeedAdjuct(double& vx,double& vy,double& vw);
	double reachSpeedAdjust(double raw_speed,double reach_speed,double dist); //���ݵ����ٶȣ�����Ŀ���ľ��룬����raw_speed���ڿ쵽���ʱ�򣬼��ٵ�Ŀ���ٶ�

	double smoothW(double e_w,double wcc_dec,double wcc_inc);
	double smoothV(double e_v,double v_dec,double v_inc);

	double m_max_vx;		//����������ٶ�(mm/s)
	double m_max_ax;		//��������߼��ٶ�(mm/s^2)
	double m_max_vw;		//�����ٶ�(rad/s)
	double m_max_aw;		//���Ǽ��ٶ�(rad/s^2)
	double m_stop_dist;		//ͣ�Ͼ��룬���������������Χ��ô������ֹͣ��mm��

	enum{MOTION_REACHED,MOTION_STRAIGHT,MOTION_TURN,MOTION_STOP,MOTION_NEAR,MOTION_COARSEREACH};
	/// Motion Type
	int				m_cur_motion_state;
	int				m_cur_motion_cnt;
	int				m_safe_cnt;

	RobotPose		m_local_target;
	RobotState		m_prev_terminal_state;
	bool			m_terminal_state_change;
	LoggerPtr			m_logger;

	LaserSafeAdjust		 m_laser_safe_handler;
};
}

#endif	// ~NRF_DYNAMIC_WINDOW_APPROACH_H
