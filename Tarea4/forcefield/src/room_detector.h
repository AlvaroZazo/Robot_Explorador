//
// Created by alumno on 14/12/22.
//
#ifndef FORCEFIELD_ROOM_DETECTOR_H
#define FORCEFIELD_ROOM_DETECTOR_H

#include "specificworker.h"
#include <cppitertools/range.hpp>
#include <cppitertools/enumerate.hpp>
#include <cppitertools/filter.hpp>
#include <cppitertools/chunked.hpp>
#include <cppitertools/sliding_window.hpp>
#include <cppitertools/combinations_with_replacement.hpp>




class Room_detector {
private:


public:
    //VARS
    AbstractGraphicViewer *viewer;

    //STRUCT DOORS
    struct Room{

        RoboCompYoloObjects::TObjects objects;

    };

    Room habitacion;
    //METHODS

    std::vector<Room> room_objects(const RoboCompYoloObjects::TObjects objects);
};


#endif //FORCEFIELD_ROOM_DETECTOR_H
