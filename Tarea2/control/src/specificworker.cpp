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
int MAX_ADV_SPEED= 1000;
float BREAKSPEED = - MAX_ADV_SPEED/3;
float TURN_ROTATION = 1;
float BASESPEED = 500;
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
        this->actualRotation = 0.2;
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


    //Laser parte frontal
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);

    //Laser parte derecha
    RoboCompLaserMulti::TLaserData copyRight(ldata.begin(), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyRight, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte izquierda
    RoboCompLaserMulti::TLaserData copyLeft(ldata.begin()+(ldata.size()/part), ldata.end());
    std::ranges::sort(copyLeft, {},&RoboCompLaserMulti::TData::dist);



    qInfo() << copy.front().dist << "Laser frente";
    qInfo() << copyLeft.front().dist<< "Laser Izquierda";
    qInfo() << copyRight.front().dist<< "Laser Izquierda";


    if (copy.front().dist <= 500 || copyLeft.front().dist <= 500  || copyRight.front().dist <= 500 ) {    //--CASO TURN
        return TURN_PROC_method(copyLeft.front().dist,copyRight.front().dist);

    } else if(copy.front().dist > 1500 && copyLeft.front().dist > 1000 && copyRight.front().dist > 1000 ){ //--SPIRAL CASE
        return std::make_tuple(State::SPIRAL, this->actualSpeed + 50 , this->actualRotation + 0.5);
    }

    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);                                          //--FORWARD CASE


}




            //----------------------------FORWARD METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::FORWARD_method(const RoboCompLaserMulti::TLaserData &ldata) {
    //std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;


    //Laser parte frontal
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);

    //Laser parte derecha
    RoboCompLaserMulti::TLaserData copyRight(ldata.begin(), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyRight, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte izquierda
    RoboCompLaserMulti::TLaserData copyLeft(ldata.begin()+(ldata.size()/part), ldata.end());
    std::ranges::sort(copyLeft, {},&RoboCompLaserMulti::TData::dist);



    qInfo() << copy.front().dist << "Laser frente";
    qInfo() << copyLeft.front().dist<< "Laser Izquierda";
    qInfo() << copyRight.front().dist<< "Laser Derecha";


    //El robot se acerca demasiado a una pared.
    if (copy.front().dist <= 500 || copyLeft.front().dist <= 500  || copyRight.front().dist <= 500 ) {   //--CASO TURN
        return TURN_PROC_method(copyLeft.front().dist,copyRight.front().dist);

    }else if(copy.front().dist > 1500 && copyLeft.front().dist > 1000 && copyRight.front().dist > 1000 ){ //--SPIRAL CASE
        return std::make_tuple(State::SPIRAL, this->actualSpeed + 50 , this->actualRotation + 0.5);
}
    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);                                         //--FORWARD CASE
}




                //----------------------------TURN METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::TURN_method(const RoboCompLaserMulti::TLaserData &ldata) {
    const int part = 3;



   //Laser parte frontal
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);

    //Laser parte derecha
    RoboCompLaserMulti::TLaserData copyRight(ldata.begin(), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyRight, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte izquierda
    RoboCompLaserMulti::TLaserData copyLeft(ldata.begin()+(ldata.size()/part), ldata.end());
    std::ranges::sort(copyLeft, {},&RoboCompLaserMulti::TData::dist);



    qInfo() << copy.front().dist << "Laser frente";
    qInfo() << copyLeft.front().dist<< "Laser Izquierda";
    qInfo() << copyRight.front().dist<< "Laser Izquierda";



    if(copy.front().dist > 1500 && copyLeft.front().dist > 1000 && copyRight.front().dist > 1000 ){ //--SPIRAL CASE
        return std::make_tuple(State::SPIRAL, this->actualSpeed + 50 , this->actualRotation + 0.5);

    }else if (copy.front().dist > 500 || copyLeft.front().dist > 500  || copyRight.front().dist > 500 ) {  //--FORWARD CASE
        return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);

    }else{                                                                                                 //--TURN CASE

        /*ELEGIR ENTRE TURN Y WALL
                -->Podria crear un temporizador que alterne entre los dos modos cada cierto tiempo.
        */


        return TURN_PROC_method(copyLeft.front().dist,copyRight.front().dist);
    }

}

                //----------------------------SPIRAL METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::SPIRAL_method(const RoboCompLaserMulti::TLaserData &ldata) {
    std::tuple<SpecificWorker::State, float, float> result;

    const int part = 3;

    //Laser parte frontal
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);

    //Laser parte derecha
    RoboCompLaserMulti::TLaserData copyRight(ldata.begin(), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyRight, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte izquierda
    RoboCompLaserMulti::TLaserData copyLeft(ldata.begin()+(ldata.size()/part), ldata.end());
    std::ranges::sort(copyLeft, {},&RoboCompLaserMulti::TData::dist);



    qInfo() << copy.front().dist << "Laser frente";
    qInfo() << copyLeft.front().dist<< "Laser Izquierda";
    qInfo() << copyRight.front().dist<< "Laser Izquierda";


    if (copy.front().dist <= 500 || copyLeft.front().dist <= 500  || copyRight.front().dist <= 500 ) {  //--CASO TURN
        //reset speed
        this->actualRotation = 0;
        this->actualSpeed = BASESPEED;

        return TURN_PROC_method(copyLeft.front().dist,copyRight.front().dist);

    } else if (copy.front().dist > 1500 && copyLeft.front().dist > 1000 && copyRight.front().dist > 1000  ) {
        return std::make_tuple(State::SPIRAL, this->actualSpeed + 50 , this->actualRotation + 0.5);     //--SPIRAL CASE
    }
    //reset speed
    this->actualRotation = 0;
    this->actualSpeed = BASESPEED;
    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);                    //--FORWARD CASE
}



                //----------------------------WALL METHOD-----------------------------//

SpecificWorker::Action SpecificWorker::WALL_method(const RoboCompLaserMulti::TLaserData &ldata)
{
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;


    //Laser parte frontal
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);

    //Laser parte derecha
    RoboCompLaserMulti::TLaserData copyRight(ldata.begin(), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyRight, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte izquierda
    RoboCompLaserMulti::TLaserData copyLeft(ldata.begin()+(ldata.size()/part), ldata.end());
    std::ranges::sort(copyLeft, {},&RoboCompLaserMulti::TData::dist);



    qInfo() << copy.front().dist << "Laser frente";
    qInfo() << copyLeft.front().dist<< "Laser Izquierda";
    qInfo() << copyRight.front().dist<< "Laser Izquierda";



    return std::make_tuple(State::FORWARD, MAX_ADV_SPEED, 0);
}

SpecificWorker::Action SpecificWorker::TURN_PROC_method(float laserLeft, float laserRight)  {
    bool turnRigth = false;
    float rotation;

    if(laserRight > laserLeft )
        turnRigth = true;

    //--TURN CASE
    if (turnRigth) {
        rotation = TURN_ROTATION;
    }else {
        rotation = -TURN_ROTATION;
    }

    return std::make_tuple(State::TURN, BREAKSPEED, rotation);
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