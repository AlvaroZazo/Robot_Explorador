/*
 *    Copyright (C) 2022 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
	\brief
	@author authorname
*/



#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>

class SpecificWorker : public GenericWorker
{
    Q_OBJECT
public:
    SpecificWorker(TuplePrx tprx, bool startup_check);
    ~SpecificWorker();
    bool setParams(RoboCompCommonBehavior::ParameterList params);

public slots:
            void compute();
    int startup_check();
    void initialize(int period);

private:

    bool startup_check_flag;
    enum class State {IDLE, FORWARD, TURN, WALL, SPIRAL};
    State state = State::IDLE;

    using Action = std::tuple<State, float, float>;
    Action IDLE_method(const RoboCompLaserMulti::TLaserData &ldata);
    Action FORWARD_method(const RoboCompLaserMulti::TLaserData &ldata);
    Action TURN_method(const RoboCompLaserMulti::TLaserData &ldata);
    Action WALL_method(const RoboCompLaserMulti::TLaserData &ldata);
    Action SPIRAL_method(const RoboCompLaserMulti::TLaserData &ldata);

};

#endif