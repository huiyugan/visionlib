/** \file
	\brief Base interface class of motion control.
	\author NanJiang Robot Inc.
	\date 2016-02-26
	\version 0.0.1
*/

#ifndef NRF_ABSTRACT_MOTION_CONTROL_H
#define NRF_ABSTRACT_MOTION_CONTROL_H

//////////////////////////////////////////////////////////////////////////
// include files
#include <common/types.h>
#include <common/utils.h>
#include <iostream>
#include <iomanip>
namespace NJRobot
{

/** \brief Base interface class of motion control.*/
// �˶������ࣺ����һ��·���͵�ǰλ�ã�����������ٶȣ�ʹ�������ظ�����·������
class AbstractMotionControl {
public:
	/// Constructor
	AbstractMotionControl();
	/// Destructor
	virtual ~AbstractMotionControl() {}

	/// Set current laser
	void setObstaclePoints(const PointList & obs){
		m_obs_points = obs;
	}

	void setObstacleData(const ObstacleData& od){
		m_obs_data = od;
	}

	void setProcessLoop(double t){
		m_process_loop = t;
	}

	// path ��һ��������һ��Ŀ����ǰλ�ã��ڶ������ǵ�ǰҪȥ��λ�ã�is_real_final��path[1]�ǲ������յ�λ��
	void setPath(const std::vector<RobotState>& path,bool is_real_final=false){
		if(path.size()<2){
			std::cout<<"ERROR: Invalid path."<<std::endl;
			return;
		}
		m_robot_path = path;
		m_cur_target = path[1];
		m_is_real_final = is_real_final;
		m_is_reached = false;
	}


	// ����ģʽ:ͨ��setTargetState() ����Ŀ��״̬��ͨ��DoMotionControl()ʵʱ���뵱ǰ״̬�������˶�����
	void doMotionControl(const RobotState& initial_state){
		// 1. init
		m_cur_state = initial_state;
		// 2. generate available velocity
		computeCurSpeed();
		RobotSpeed FSMspeed = m_cur_speed;
		// 3. keep safety
		safeAdjust();
		RobotSpeed finalSpeed = m_cur_speed;
		
		m_prev_speed = m_cur_speed;
		m_obs_points.clear();
	}

	void setMaxVelocity(double max_v){
		m_max_velocity = max_v;
	}
	double getMaxVelocity()
	{
		return m_max_velocity;
	}

	//�Ƿ���ͣ��
	bool obsStopOccur(){
		return m_obs_stop_flag;
	}

	/// Get current velocity
	// (vx, vy, w) -> (m/s, m/s, rad/s)
	const RobotSpeed& getCurSpeed() const{
		return m_cur_speed;
	}

	void setPrevSpeed(const RobotSpeed& speed){
		m_prev_speed = speed;
	}

	const RobotSpeed& getPrevSpeed() const{
		return m_prev_speed;
	}

	void setRobotShape(const RobotShapeRect& shape){
		m_robot_shape = shape;
	}

	RobotShapeRect getRobotShape(){
		return m_robot_shape;
	}

	bool isReached(){
		return m_is_reached;
	}

	void setMinTurnRadius(double a){
		m_min_turn_radius = a;
	}

protected:
	virtual void computeCurSpeed()=0;
	virtual void safeAdjust()=0;

	PointList							m_obs_points; //��ǰ�����ϰ����,��������������ϵ
	ObstacleData					m_obs_data;  //��ǰ���������ϰ����

	RobotSpeed					    m_cur_speed;    //��ǰ��Ҫ�Ļ������ٶ� [m/s, m/s, rad/s]
	RobotSpeed					    m_prev_speed; 

	RobotShapeRect					m_robot_shape;

	RobotState						m_cur_state;  //�����˵�ǰ��λ��
	RobotState						m_cur_target;     //��������һ��Ŀ���
	std::vector<RobotState>	m_robot_path;     //Ҫ�������ߵ�·��
	bool									m_is_real_final;

	bool									m_obs_stop_flag;  //�������Ƿ���ͣ��
	bool									m_is_reached; //�������Ƿ񵽵�

	double								m_process_loop; //�������� ��λs
	double								m_max_velocity; //���������ٶ�

	double								m_min_turn_radius; //��Сת��뾶

	double PredictCollision(double v , double w , const Point &obstacle , const Line& line);
	double PredictCollision(double v , double w);

};
}
#endif	// ~NRF_ABSTRACT_MOTION_CONTROL_H