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
#include "specificworker.h"

/**
* \brief Default constructor
*/

enum class State {IDLE, FORWARD, TURN, FOLLOW_WALL, SPIRAL};
int MAX_ADV_SPEED=1000;

SpecificWorker::SpecificWorker(TuplePrx tprx, bool startup_check) : GenericWorker(tprx)
{
    this->startup_check_flag = startup_check;
    // Uncomment if there's too many debug messages
    // but it removes the possibility to see the messages
    // shown in the console with qDebug()
//	QLoggingCategory::setFilterRules("*.debug=false\n");
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
    std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
    return true;
}

void SpecificWorker::initialize(int period)
{
    std::cout << "Initialize worker" << std::endl;
    this->Period = period;
    if(this->startup_check_flag)
    {
        this->startup_check();
    }
    else
    {
        timer.start(Period);
    }
}

void SpecificWorker::compute()
{
    RoboCompLaserMulti::TLaserData ldata;
    try
    { ldata = lasermulti_proxy->getLaserData(1);}
    catch (const Ice::Exception &e) {std::cout << e.what() << std::endl; return;}

//    const int part = 3;
//
//    RoboCompLaserMulti::TLaserData  copy;
//    copy.assign(ldata.begin()+ldata.size()/part, ldata.end()-ldata.size()/part);
//    std::ranges::sort(copy, {},&RoboCompLaserMulti::TData::dist);
//    qInfo() << copy.front().dist;
//    if(copy.front().dist < 500)
//    {
//        state = State::TURN;
//       // addv = 700;
//       // rot = 0.7;
//       //qInfo() << copy.front().dist;
//    }
//    else
//    {
//        addv = 600;
//        rot = 0;
//    }
////        for (const auto &l :ldata)
////            qInfo() << l.angle << l.dist;
////
////        qInfo() << "--------------";
//}
    //Guardamos en la tupla el movimiento que queremos que realice.
    std::tuple<State, float, float> result;    //State -> enum class

    //Decidimos en funcion del valor guardado en la tupla, el movimiento que debe hacer.
    switch(std::get<State>(result))
    {
        case State::IDLE:

            break;
        case State::FORWARD:
            result = FORWARD_method(ldata);
            break;
        case State::TURN:
            break;
        case State::WALL:
            break;
        case State::SPIRAL:
            break;
    }

    //Bloque para enviar las velocidades al robot.
    auto &[_, adv, rot]  = result;
    try
    { differentialrobotmulti_proxy->setSpeedBase(1,adv,rot);}
    catch (const Ice::Exception &e) {std::cout << e.what() << std::endl; }

}

int SpecificWorker::startup_check()
{
    std::cout << "Startup check" << std::endl;
    QTimer::singleShot(200, qApp, SLOT(quit()));
    return 0;
}


//-------------------------
//METODOS IMPLEMENTADOS
//-------------------------
SpecificWorker::Action SpecificWorker::FORWARD_method(const RoboCompLaserMulti::TLaserData &ldata)
{
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;
    int binario = std::rand()%2;

    RoboCompLaserMulti::TLaserData  copy;
    copy.assign(ldata.begin()+ldata.size()/part, ldata.end()-ldata.size()/part);
    std::ranges::sort(copy, {},&RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;
    if(copy.front().dist < 200)
    {
        std::get<0>(result) = State::TURN;
        std::get<1>(result) = 0;
        if(binario==0)
            std::get<2>(result) = 0.5;
        else
            std::get<2>(result) = -0.5;
    } else if(copy.front().dist>1000){
        std::get<0>(result) = State::SPIRAL;
        std::get<1>(result) = 0;
        //std::get<2>(result) =formula espiral;
    } else {
        std::get<0>(result) = State::IDLE;
        std::get<1>(result) = MAX_ADV_SPEED;
        std::get<2>(result) = 0;
    }
    return result;
}


SpecificWorker::Action SpecificWorker::TURN_method(const RoboCompLaserMulti::TLaserData &ldata)
{
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;
    int binario = std::rand()%2;

    RoboCompLaserMulti::TLaserData  copy;
    copy.assign(ldata.begin()+ldata.size()/part, ldata.end()-ldata.size()/part);
    std::ranges::sort(copy, {},&RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;
    if(copy.front().dist < 200)//SPACEAHEAD
    {
        std::get<0>(result) = State::TURN;
        std::get<1>(result) = 0;
        std::get<2>(result) = binario;
    } else if(copy.front().dist>1000){
        std::get<0>(result) = State::SPIRAL;
        std::get<1>(result) = 0;
        //std::get<2>(result) =formula espiral;
    } else {
        std::get<0>(result) = State::IDLE;
        std::get<1>(result) = MAX_ADV_SPEED;
        std::get<2>(result) = 0;
    }
    return result;
}


/**************************************/
// From the RoboCompDifferentialRobot you can call this methods:
// this->differentialrobot_proxy->correctOdometer(...)
// this->differentialrobot_proxy->getBasePose(...)
// this->differentialrobot_proxy->getBaseState(...)
// this->differentialrobot_proxy->resetOdometer(...)
// this->differentialrobot_proxy->setOdometer(...)
// this->differentialrobot_proxy->setOdometerPose(...)
// this->differentialrobot_proxy->setSpeedBase(...)
// this->differentialrobot_proxy->stopBase(...)

/**************************************/
// From the RoboCompDifferentialRobot you can use this types:
// RoboCompDifferentialRobot::TMechParams

/**************************************/
// From the RoboCompLaser you can call this methods:
// this->laser_proxy->getLaserAndBStateData(...)
// this->laser_proxy->getLaserConfData(...)
// this->laser_proxy->getLaserData(...)

/**************************************/
// From the RoboCompLaser you can use this types:
// RoboCompLaser::LaserConfData
// RoboCompLaser::TData
