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
int MAX_ADV_SPEED= 700;

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

    //Guardamos en la tupla el movimiento que queremos que realice.
    std::tuple<State, float, float> result;    //State -> enum class

    //Decidimos en funcion del valor guardado en la tupla, el movimiento que debe hacer.
    switch(std::get<State>(result))
    {

               case State::TURN:
                   result = TURN_method(ldata);
                   break;
/*
                case State::IDLE:
                   result = IDLE_method(ldata);
                  break;
               case State::FORWARD:
                   result = FORWARD_method(ldata);
                   break;

               case State::WALL:
                   result = WALL_method(ldata);
                   break;
               case State::SPIRAL:
                   result = SPIRAL_method(ldata);
                   break;
               */
        default:                        //En caso de ser la primera llamada.
            result = IDLE_method(ldata);
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


//-----------------------------HECHO------------------------------//
//----------------------------TOCHECK-----------------------------//
SpecificWorker::Action SpecificWorker::IDLE_method(const RoboCompLaserMulti::TLaserData &ldata) {
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;

    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    //MODIFICACION PARA REALIZAR EL FORWARD (Modifico la tupla result)
    std::get<1>(result) = 400;


    //COMPROBACIÓN DEL ESTADO ACTUAL DEL ROBOT (Se modifica el valor de la tupla State)
    if (copy.front().dist <= 200) {                                           //CASO DISTANCIA REDUCIDA igual o menor 200 CAMBIA EL ESTADO A TURN.
        std::get<0>(result) = State::TURN;

    } else if ((copy.front().dist > 200) && (copy.front().dist <= 500) ) {    // CASO DISTANCIA MAYOR A 200 Y MENOR 500 CAMBIA EL ESTADO A WALL.
        std::get<0>(result) = State::WALL;

    } else if ((copy.front().dist > 500) && (copy.front().dist <= 1000) ) {   // CASO DISTANCIA MAYOR A 200 Y MENOR 1000 CAMBIA EL ESTADO A FORWARD.
        std::get<0>(result) = State::FORWARD; //No tendria efecto sobre el estado actual

    } else if (copy.front().dist > 1000) {                                    //CASO DISTANCIA MAYOR 1000 CAMBIA EL ESTADO A SPIRAL.
        std::get<0>(result) = State::SPIRAL;

    } else {
        std::get<0>(result) = State::IDLE;                                 //CASO OCIOSO DE NO ESTAR REFLEJADO.

    }
    return result;
}



//-----------------------------HECHO------------------------------//
//----------------------------TOCHECK-----------------------------//
SpecificWorker::Action SpecificWorker::FORWARD_method(const RoboCompLaserMulti::TLaserData &ldata) {
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;

    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    //MODIFICACION PARA REALIZAR EL FORWARD (Modifico la tupla result)
    std::get<1>(result) = MAX_ADV_SPEED;

    //COMPROBACIÓN DEL ESTADO ACTUAL DEL ROBOT (Se modifica el valor de la tupla State)
    if (copy.front().dist <= 200) {                                           //CASO DISTANCIA REDUCIDA igual o menor 200 CAMBIA EL ESTADO A TURN.
        std::get<0>(result) = State::TURN;

    } else if ((copy.front().dist > 200) && (copy.front().dist <= 500) ) {    // CASO DISTANCIA MAYOR A 200 Y MENOR 500 CAMBIA EL ESTADO A WALL.
        std::get<0>(result) = State::WALL;

    } else if ((copy.front().dist > 500) && (copy.front().dist <= 1000) ) {   // CASO DISTANCIA MAYOR A 200 Y MENOR 1000 CAMBIA EL ESTADO A FORWARD.
        std::get<0>(result) = State::FORWARD; //No tendria efecto sobre el estado actual

    } else if (copy.front().dist > 1000) {                                    //CASO DISTANCIA MAYOR 1000 CAMBIA EL ESTADO A SPIRAL.
        std::get<0>(result) = State::SPIRAL;

    } else {
        std::get<0>(result) = State::IDLE;                                 //CASO OCIOSO DE NO ESTAR REFLEJADO.

    }
    return result;
}

//-----------------------------HECHO------------------------------//
//----------------------------TOCHECK-----------------------------//
SpecificWorker::Action SpecificWorker::TURN_method(const RoboCompLaserMulti::TLaserData &ldata)
{
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;
    int binario = std::rand() % 2;

    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    //MODIFICACION PARA REALIZAR EL FORWARD (Modifico la tupla result)
    //El robot gira 45 grados
    std::get<1>(result) = 0;
    if(binario == 0) {
        std::get<2>(result) = M_PI/4; // (todo) Lo he puesto positivo de momento
    }else{
        std::get<2>(result) = M_PI/4;
    }
    //COMPROBACIÓN DEL ESTADO ACTUAL DEL ROBOT (Se modifica el valor de la tupla State)
    if (copy.front().dist <= 200) {                                           //CASO DISTANCIA REDUCIDA igual o menor 200 CAMBIA EL ESTADO A TURN.
        std::get<0>(result) = State::TURN;

    } else if ((copy.front().dist > 200) && (copy.front().dist <= 500) ) {    // CASO DISTANCIA MAYOR A 200 Y MENOR 500 CAMBIA EL ESTADO A WALL.
        std::get<0>(result) = State::WALL;

    } else if ((copy.front().dist > 500) && (copy.front().dist <= 1000) ) {   // CASO DISTANCIA MAYOR A 200 Y MENOR 1000 CAMBIA EL ESTADO A FORWARD.
        std::get<0>(result) = State::FORWARD; //No tendria efecto sobre el estado actual

    } else if (copy.front().dist > 1000) {                                    //CASO DISTANCIA MAYOR 1000 CAMBIA EL ESTADO A SPIRAL.
        std::get<0>(result) = State::SPIRAL;

    } else {
        std::get<0>(result) = State::TURN;                                 //CASO OCIOSO DE NO ESTAR REFLEJADO.

    }
    return result;

}

//----------------------------------TODO-----------------------------//
//----------------------------------TOCHECK--------------------------//
SpecificWorker::Action SpecificWorker::WALL_method(const RoboCompLaserMulti::TLaserData &ldata)
{
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;


    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    //MODIFICACION PARA REALIZAR EL FORWARD (Modifico la tupla result)
    // EL robot baja su velocidad y gira de forma que quede en paralelo con el obtaculo detectado
    std::get<1>(result) = 300;
    std::get<2>(result) = M_PI/2;

    //COMPROBACIÓN DEL ESTADO ACTUAL DEL ROBOT (Se modifica el valor de la tupla State)
    if (copy.front().dist <= 200) {                                           //CASO DISTANCIA REDUCIDA igual o menor 200 CAMBIA EL ESTADO A TURN.
        std::get<0>(result) = State::TURN;

    } else if ((copy.front().dist > 200) && (copy.front().dist <= 500) ) {    // CASO DISTANCIA MAYOR A 200 Y MENOR 500 CAMBIA EL ESTADO A WALL.
        std::get<0>(result) = State::WALL;

    } else if ((copy.front().dist > 500) && (copy.front().dist <= 1000) ) {   // CASO DISTANCIA MAYOR A 200 Y MENOR 1000 CAMBIA EL ESTADO A FORWARD.
        std::get<0>(result) = State::FORWARD; //No tendria efecto sobre el estado actual

    } else if (copy.front().dist > 1000) {                                    //CASO DISTANCIA MAYOR 1000 CAMBIA EL ESTADO A SPIRAL.
        std::get<0>(result) = State::SPIRAL;

    } else {
        std::get<0>(result) = State::IDLE;                                 //CASO OCIOSO DE NO ESTAR REFLEJADO.

    }
    return result;
}

//----------------------------------TODO-----------------------------//
//----------------------------------TOCHECK--------------------------//
SpecificWorker::Action SpecificWorker::SPIRAL_method(const RoboCompLaserMulti::TLaserData &ldata)
{
    std::tuple<SpecificWorker::State, float, float> result;
    const int part = 3;

    //COPIA DE LOS VALORES DEL LASER
    RoboCompLaserMulti::TLaserData copy;
    copy.assign(ldata.begin() + ldata.size() / part, ldata.end() - ldata.size() / part);
    std::ranges::sort(copy, {}, &RoboCompLaserMulti::TData::dist);
    qInfo() << copy.front().dist;

    //MODIFICACION PARA REALIZAR EL SPIRAL (Modifico la tupla result)
    //modificaremos su valor de rotación griando el robot 30º aprox 0,524 radianes y mantiene una velocidad constante de 500
    std::get<1>(result) = 500;
    std::get<2>(result) = std::get<2>(result) + 0.524;


    //COMPROBACIÓN DEL ESTADO ACTUAL DEL ROBOT (Se modifica el valor de la tupla State)
    if (copy.front().dist <= 200) {                                           //CASO DISTANCIA REDUCIDA igual o menor 200 CAMBIA EL ESTADO A TURN.
        std::get<0>(result) = State::TURN;

    } else if ((copy.front().dist > 200) && (copy.front().dist <= 500) ) {    // CASO DISTANCIA MAYOR A 200 Y MENOR 500 CAMBIA EL ESTADO A WALL.
        std::get<0>(result) = State::WALL;

    } else if ((copy.front().dist > 500) && (copy.front().dist <= 1000) ) {   // CASO DISTANCIA MAYOR A 200 Y MENOR 1000 CAMBIA EL ESTADO A FORWARD.
        std::get<0>(result) = State::FORWARD; //No tendria efecto sobre el estado actual

    } else if (copy.front().dist > 1000) {                                    //CASO DISTANCIA MAYOR 1000 CAMBIA EL ESTADO A SPIRAL.
        std::get<0>(result) = State::SPIRAL;

    } else {
        std::get<0>(result) = State::IDLE;                                 //CASO OCIOSO DE NO ESTAR REFLEJADO.

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