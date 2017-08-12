/** \file
	\brief Path handler packages path planning as well as path optimization, and output a list of path node which can be followed
	\author NanJiang Robot Inc.
	\date 2016-02-26
	\version 0.0.1
*/

#pragma once
#include <pathplan/abstract_path_plan.h>
#include <map/traval_map.h>
#include <common/logger.h>

#include "map_path_optimize.h"

namespace NJRobot{

class CAutoNavPathHandler{
public:
	CAutoNavPathHandler();

	~CAutoNavPathHandler();

	void setSafeDist(double d){
		m_safe_dist = d;
	}

	double getSafeDist(){
		return m_safe_dist;
	}

	void setTarget(const RobotState& target){
		m_tar_state = target;
		m_ref_path_seted = false;
		m_is_path_planed = false;
	}
	
	RobotState getTarget(){
		return m_tar_state;
	}

	void setObsData(const PointList& obs){
		m_obs_point = obs;
	}

	void setRefPath(const RobotPath& path){
		m_ref_path = path;
		m_ref_path_seted = true;
	}

	RobotPath getPath(){
		return m_path;
	}

	bool hasPath(){
		return m_is_path_planed;
	}

	AbstractPathPlan* getPathPlaner(){
		return m_path_planer;
	}

	bool loadMapFile(const std::string & map);

	void process(const RobotState& cur_state,const PointList& obs);

	//���·�����Ƿ�����ϰ���  -1:��ʾ�������·��
	bool pathSafeCheck(const RobotPath& path,double maxCheckDist=-1);

protected:
	LoggerPtr				m_logger;

	// ������Ϣ
	RobotState				m_tar_state;		//��ǰĿ���
	RobotPath				m_ref_path;			//�ο�·�����滮����·��������ο�·�����̫��
	bool					m_ref_path_seted;	//�Ƿ������˲ο�·��
	FixedQueue<RobotState>	m_history_targets;  //��ʷĿ���
	double					m_safe_dist;

	// ����״̬��Ϣ
	RobotState				m_cur_state;		//�����˵�ǰλ��
	PointList				m_obs_point;		//ʵʱ�����ϰ���
	
	// �м䴦�����
	int						m_auto_path_danger_cnt; //�����滮��·������һ���������ڵ������¹滮·��

	// ������
	RobotPath				m_path;				//�滮����·��
	bool					m_is_path_planed;	//�Ƿ���·��

private:
	AbstractPathPlan*		m_path_planer;
	AbstractPathOptimize*	m_path_optimizer;
	MapPathOptimize			m_path_optimize_handler;

	/// ��������ʵ�ֺ��� //////
	//·���滮������ʼ��ʱ�򣬹滮һ��·��
	void doPathPlan();
	//·���Ż�
	void doPathOptimize();
	//·�����£�·���滮���Ժ���·���ߣ�ʵʱ����·��
	void doAutoNavPathUpdate();
	//��̬��Ƥ���Ż�·��
	void elasticBandOptimize();
	//ȥ��·����������ĵ㣬��Ϊֻ�е�·��size��2��ʱ��Ż���е����������Ҫ�㹻�ĵ���ʱ�䡣
	void prunThePathTail();
	// ���¿�ͨ�е�ͼ
	void updateTravelMap();


};




}