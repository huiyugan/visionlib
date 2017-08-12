#include <navigation/laser_safe_adjust.h>
#include <common/utils.h>
#include <common/timer.h>
#include <common/std_out.h>

namespace NJRobot{


LaserSafeAdjust::LaserSafeAdjust(void):AbstractSafeAdjust<PointList>()
, m_has_obs_front(false)
, m_has_obs_left(false)
, m_has_obs_right(false)
{
}

LaserSafeAdjust::~LaserSafeAdjust(void)
{
}

void LaserSafeAdjust::dangerCheck( const PointList & obs,const RobotSpeed& actual_speed )
{
	double prev_v = actual_speed.vx;
	// check danger
	m_has_obs_front = m_has_obs_left = m_has_obs_right = false;

	m_closest_point = Point(DBL_MAX,DBL_MAX);

	double half_width = m_robot_shape.left();
	double half_length = m_robot_shape.front();
	double stop_dist_turn = 0.010; //0.01m
	double danger_dist_turn  = std::max(half_width,half_length)+stop_dist_turn;
	double max_check_dist = clip(linearEquation(0.8,3, 0.4,2, prev_v),2.0,5.0);//����ϰ�������룬����ٶȺܿ죬�������Ҫ�Ӵ�  v:0.4m/s -> d=2m   v:0.8m/s -> d=3m

	for(int i=0;i<obs.size();i++){
		Point pt = obs[i];
		double ox = pt.x;
		double oy = pt.y;
		double theta_deg = rad2deg(pt.dir());

		//̫�ڳ�������ĵ㱻��Ϊ�����
		if(ox+0.100<half_length  && fabs(oy)+0.100<half_width)
		{
			continue;
		}
		//ǰ���Ƿ����ϰ���еĻ���������Ҫ���ٻ���ͣ��
		if(fabs(oy)<=half_width && ox>0.010 &&ox-half_length<max_check_dist) //max_check_dist����ľͲ�������
		{
			m_has_obs_front = true;
			if(ox<m_closest_point.x) m_closest_point = pt;
		}
		//������Χ�Ƿ����ϰ���еĻ������˲���ת��
		if(hypot(ox,oy)<danger_dist_turn)
		{
			if(m_has_obs_left==false && theta_deg>-10)
			{
				m_has_obs_left = true;
				m_obs_point_left = pt;
			}
			if(m_has_obs_right==false && theta_deg<10)
			{
				m_has_obs_right = true;
				m_obs_point_right = pt;
			}
		}
	}
}

void LaserSafeAdjust::safeSpeedAdjust(RobotSpeed& send_speed,const PointList & obs,const RobotSpeed& actual_speed)
{
	m_stop_flag = false;
	m_slow_flag = false;
	// check danger
	dangerCheck(obs,actual_speed);

	double& vx = send_speed.vx;
	double& vy = send_speed.vy;
	double& vw = send_speed.w;

	double prev_v = actual_speed.vx;
	double prev_w = actual_speed.w;

	// handle vw
	if(m_has_obs_left && vw>0)
	{
		COUT_COLOR(getCurTimeStr()+" Laser STOP! Danger LEFT.("<<m_obs_point_left<<")",COLOR_BLUE);
		vw=0;
	}
	if(m_has_obs_right && vw<0)
	{
		vw=0;
		COUT_COLOR(getCurTimeStr()+" Laser STOP! Danger RIGHT.("<<m_obs_point_right<<")",COLOR_BLUE);
	}
	// handle vx
	double stop_dist =  linearEquation(0.15,0.10,0.3,0.20,vx);
	stop_dist = clip(stop_dist,0.08,0.2);
	
	if(m_has_obs_front){
		double cache_dist = m_closest_point.x - m_robot_shape.front();  //������룬������ǰ��������ϰ���ľ���
		if(cache_dist<0) cache_dist=0;

		if(cache_dist<=stop_dist){ //�����û����صģ�һ��Σ�վ���̫С��������ô������ͣ
			m_stop_flag = true;
			COUT_COLOR(getCurTimeStr()+" Laser STOP! Danger FRONT. dist:"<<cache_dist,COLOR_BLUE);
			LOG_INFO(m_logger,"Laser STOP! Danger FRONT. dist"<<cache_dist);
			vx = 0.0;vy = 0.0;
		}else if(m_closest_point.x<=getTargetPoint().x){ //�ϰ��ﳬ��Ŀ��㼤�ⲻ����
			if(vx>0.5){ //laser slow �ǲ���Ҫ�ģ���Ϊ��������·��������ʱ���ֱ��ͣ��
				//�ٶȸĳɴ���0.1����ֵΪ0.1�ǻ��ڵ�ǰturn�ǵ��ٶȲ��󣬼���Ϊ0.0�����ԣ�
				//��Ϊ�˷�ֹturn��ʱ��turnʱ����ͻȻǰ�������ϰ����������ʱ�ģ����٣���ʱ����û�б�Ҫ��
				double newV = generalSlow(vx,cache_dist,prev_v);
				if(newV<vx){
					m_slow_flag = true;
					COUT_COLOR(getCurTimeStr()+" Laser slow. ("<<m_closest_point<<") Dist:"<<cache_dist,COLOR_DARKBLUE);
					LOG_INFO(m_logger,"Laser slow. ("<<m_closest_point<<") Dist:"<<cache_dist);
				}
				vx = newV;
			}
		}else{
			LOG_INFO(m_logger,"Has front obs point ("<<m_closest_point<<") Not change speed because is not on the path. Local tar:"<<getTargetPoint());
			//COUT_COLOR(getCurTimeStr()+" Find front obs, not stop.",COLOR_DARKBLUE);
		}
	}
}

//bool LaserSafeAdjust::isRobotSlow( RobotSpeed& send_speed,const PointList & obs,const RobotSpeed& actual_speed )
//{
//	m_slow_flag = false;
//	m_stop_flag = false;
//	dangerCheck(obs,actual_speed);
//
//	double& vx = send_speed.vx;
//	double& vy = send_speed.vy;
//	double& vw = send_speed.w;
//
//	double prev_v = actual_speed.vx;
//	double prev_w = actual_speed.w;
//	double stop_dist =  linearEquation(0.15,0.1,0.3,0.20,vx);
//	if (stop_dist>0.2)
//	{
//		stop_dist=0.2;
//	}else if (stop_dist<0.08)//���������׼ȷ�ԣ�ǰ�����پ����С
//	{
//		stop_dist = 0.08;
//	}
//	if(m_has_obs_front)
//	{
//		double cache_dist = m_closest_point.x-m_vehicle_length/2; 
//		if(cache_dist<0) cache_dist=0;
//		if(cache_dist<=stop_dist) 
//		{
//			m_stop_flag = true;
//		}
//		else if(m_closest_point.x<=getRobotState().x&&cache_dist>stop_dist&&cache_dist<1.0)	
//		{
//
//			m_slow_flag = true;					
//		}
//	}
//	return m_slow_flag;
//}



}