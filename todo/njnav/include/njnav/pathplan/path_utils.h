/** \file
	\brief Privides utilities to handle path
	\author NanJiang Robot Inc.
	\date 2016-02-26
	\version 0.0.1
*/

#pragma once

#include <common/types.h>

namespace NJRobot{
	// ȡ·����ǰk����·���� cut_path���Ƿ�ضϵ�ǰ·���Ըպõõ�k�׵�·�������ǲ��ضϵõ���С��k����С��·����
	RobotPath subPath(const RobotPath& path, double max_len, bool cut_path = false);
	
	// ��һȺ���������
	PointList inflatePointList( const PointList & pointlist_raw,double inflate_dist=0.0,double reso=0.1,double downsample = 1 );

	//�ж�����·���Ƿ���ͬһ���򣬼��ж��Ƿ�滮�˻�ͷ·
	double pathAngleDiff(const RobotPath& path1,const RobotPath& path2, double check_dist=1);

	// �ж��ϰ����ͼ�ϵ�ֱ���Ƿ����
	bool lineFreeCheck( const Line& line , const GridMap*const map_ptr, double free_val);
	
	// �ж��ϰ����ͼ�ϵ�·���Ƿ����
	bool pathFreeCheck( const RobotPath& path , const GridMap*const map_ptr, double free_val);



}