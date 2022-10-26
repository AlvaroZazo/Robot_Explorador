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
int MAX_ADV_SPEED= 600;
float BREAKSPEED = - MAX_ADV_SPEED/3;
float MAX_ROT_SPEED = 3.14;

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
        this->actualRotation = 0;
        this->actualSpeed = 500;
        this->result = std::make_tuple(State::IDLE,actualSpeed,actualRotation);
        timer.start(Period);
    }
}

void SpecificWorker::compute()
{



    RoboCompLaserMulti::TLaserData ldata;
    try
    { ldata = lasermulti_proxy->getLaserData(1);}
    catch (const Ice::Exception &e) {std::cout << e.what() << std::endl; return;}



    //Decidimos en funcion del valor guardado en la tupla, el movimiento que debe hacer.


    switch(std::get<State>(result))
    {

               case State::TURN:
                   printf("entrando en turn");
                   result = TURN_method(ldata);
                   break;

                case State::IDLE:
                   result = IDLE_method(ldata);
                  break;

               case State::FORWARD:
                   result = FORWARD_method(ldata);
                   break;

               case State::SPIRAL:
                   result = SPIRAL_method(ldata);
                   break;

        case State::WALL:
                   result = SPIRAL_method(ldata);
                   break;

    }

    //Bloque para enviar las velocidades al robot.
    auto &[_, adv, rot]  = result;
    try
    {
        differentialrobotmulti_proxy->setSpeedBase(1,adv,rot);
    }
    catch (const Ice::Exception &e) {std::cout << e.what() << std::endl; }

}


int SpecificWorker::startup_check()
{
    std::cout << "Startup check" << std::endl;
    QTimer::singleShot(200, qApp, SLOT(quit()));
    return 0;
}



                //----------------------------IDLE METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::IDLE_method(const RoboCompLaserMulti::TLaserData &ldata) {
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;
    int binario = std::rand() % 2;
    float rotation;
    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    if (copy.front().dist <= 500 + BREAKSPEED) {                            //--TURN CASE
        if (binario == 0) {
            rotation = MAX_ROT_SPEED;

        } else  {
            rotation = -MAX_ROT_SPEED;
        }
        return std::make_tuple(State::TURN, BREAKSPEED, rotation);

    } else if(copy.front().dist > 1500){                                   //--SPIRAL CASE

        return std::make_tuple(State::SPIRAL, 400 ,this->actualRotation + 0.7 );
    }
    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);          //--FORWARD CASE


}




            //----------------------------FORWARD METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::FORWARD_method(const RoboCompLaserMulti::TLaserData &ldata) {
    //std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;
    float rotation;
    int binario = std::rand() % 2;

    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    //El robot se acerca demasiado a una pared.
    if (copy.front().dist <= 500 + BREAKSPEED) {                             //--CASO TURN
        if (binario == 0) {
            rotation = MAX_ROT_SPEED;
        }else {
            rotation = -MAX_ROT_SPEED;
        }
        return std::make_tuple(State::TURN, BREAKSPEED, rotation);

    }else if(copy.front().dist > 1500){                                     //--SPIRAL CASE
        return std::make_tuple(State::SPIRAL, 400 ,this->actualRotation + 0.7 );
}
    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);           //--FORWARD CASE
}




                //----------------------------TURN METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::TURN_method(const RoboCompLaserMulti::TLaserData &ldata) {
    const int part = 3;
    int binario = std::rand() % 2;
    float rotation;


    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;


    if(copy.front().dist > 1500){                                                   //--SPIRAL CASE
        return std::make_tuple(State::SPIRAL, 400 ,this->actualRotation + 0.7 );

    }else if (copy.front().dist > 500 + BREAKSPEED){                                //--FORWARD CASE
        return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);

    }else{                                                                          //--TURN CASE
        if (binario == 0) {
             rotation = MAX_ROT_SPEED;
        }else {
            rotation = -MAX_ROT_SPEED;
        }
    return std::make_tuple(State::TURN, BREAKSPEED, rotation);
    }

}

                //----------------------------SPIRAL METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::SPIRAL_method(const RoboCompLaserMulti::TLaserData &ldata) {
    std::tuple<SpecificWorker::State, float, float> result;
    int binario = std::rand() % 2;
    float rotation;
    const int part = 3;

    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    if (copy.front().dist <= 500 + BREAKSPEED) {                                     //--CASO TURN
        if (binario == 0) {
            rotation = MAX_ROT_SPEED;
        } else {
            rotation = -MAX_ROT_SPEED;
        }
        return std::make_tuple(State::TURN, BREAKSPEED, rotation);

    } else if (copy.front().dist > 1500) {
        return std::make_tuple(State::SPIRAL, 400, this->actualRotation + 0.7);     //--SPIRAL CASE
    }
    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);                    //--FORWARD CASE
}



                //----------------------------WALL METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::WALL_method(const RoboCompLaserMulti::TLaserData &ldata)
{
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;


    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;



    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);
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