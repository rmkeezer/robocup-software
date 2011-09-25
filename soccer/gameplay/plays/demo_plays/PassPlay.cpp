#include "PassPlay.hpp"

#include <algorithm>

using namespace std;

REGISTER_PLAY_CATEGORY(Gameplay::Plays::PassPlay, "Demos")

static bool shellLessThan(Robot *r1, Robot *r2)
{
	return r1->shell() < r2->shell();
}

Gameplay::Plays::PassPlay::PassPlay(GameplayModule *gameplay):
	Play(gameplay),
	_passer1(gameplay),
	_passer2(gameplay),
	_passer1HasBall(true)
{
}

bool Gameplay::Plays::PassPlay::run()
{
	set<OurRobot *> playRobots = _gameplay->playRobots();
	if (playRobots.size() < 2)
	{
		// No robots
		return false;
	}
	
	if(!_passer1.robot || playRobots.find(_passer1.robot) == playRobots.end())
	{
		assignNearest(_passer1.robot,playRobots,ball().pos);

	}

	if(!_passer2.robot || playRobots.find(_passer2.robot) == playRobots.end())
	{
		assignNearest(_passer2.robot,playRobots,ball().pos);
	}
	
//	_passer2.robot->move(Geometry2d::Point(0,Field_Length / 2));


	if(_passer1HasBall)
	{
		_passer2.robot->face(ball().pos);

		_passer1.setTarget(_passer2.robot->kickerBar());
		_passer1.kick_power = 100;
		_passer1.run();

		if(_passer1.done())
		{
			_passer1.restart();
			_passer1HasBall = false;
		}
	}
	else
	{
		_passer1.robot->face(ball().pos);
		_passer2.setTarget(_passer1.robot->kickerBar());
		_passer2.kick_power = 100;
		_passer2.run();
		if(_passer2.done())
		{
			_passer2.restart();
			_passer1HasBall = true;
		}
	}
	
	return true;
}