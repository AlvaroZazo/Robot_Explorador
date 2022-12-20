//
// Created by alumno on 7/12/22.
//

#ifndef FORCEFIELD_DOOR_DETECTOR_H
#define FORCEFIELD_DOOR_DETECTOR_H

#include "specificworker.h"
#include "Door.h"
#include <cppitertools/range.hpp>
#include <cppitertools/enumerate.hpp>
#include <cppitertools/filter.hpp>
#include <cppitertools/chunked.hpp>
#include <cppitertools/sliding_window.hpp>
#include <cppitertools/combinations_with_replacement.hpp>

class Door_detector {
    //Doors

private:


public:
    //VARS
    Door door;

    //METHODS

    void draw_doors(const std::vector<Door> &doors, AbstractGraphicViewer *viewer);
    std::vector<Door> door_detector(const vector<Eigen::Vector2f> &line, AbstractGraphicViewer *viewer);
};


#endif //FORCEFIELD_DOOR_DETECTOR_H
