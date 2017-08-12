/** \file
	\brief Path simplify which merge path segments to reduce the account of segments.
	\author NanJiang Robot Inc.
	\date 2016-02-26
	\version 0.0.1
*/
#pragma once
#include <pathplan/abstract_path_optimize.h>
#include <map/traval_map.h>

namespace NJRobot{

// ����path��ѡȡ��ͼ�е��ϰ���(ForbiddenLine)��ʵ�ʹ۲���ϰ�������ںϣ�Ȼ���ٽ���·���Ż�
class MapPathOptimize
{
public:
	MapPathOptimize(AbstractPathOptimize* optimizer=NULL);
	~MapPathOptimize(void);

	void setOptimizer(AbstractPathOptimize* optimizer){
		m_optimizer = optimizer;
	}

	bool loadMapFile(const std::string & mapfile);

	void optimize(RobotPath& path, const PointList& obs=PointList(0));

private:
	AbstractPathOptimize*	m_optimizer;

	TravalMapGenerator		m_obs_map;  //�洢forbiddenline
};


}
