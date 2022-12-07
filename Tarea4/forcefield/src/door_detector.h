//
// Created by alumno on 7/12/22.
//

#ifndef FORCEFIELD_DOOR_DETECTOR_H
#define FORCEFIELD_DOOR_DETECTOR_H

#include "specificworker.h"
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
    AbstractGraphicViewer *viewer;

    //STRUCT DOORS
    struct Door {

        Eigen::Vector2f peak_left;
        Eigen::Vector2f peak_right;
        Eigen::Vector2f center;


    };

    //METHODS

    void draw_doors(const std::vector<Door> &doors);
    std::vector<Door> door_detector(const vector<Eigen::Vector2f> &line, float door_dynamic_threshold);
};


#endif //FORCEFIELD_DOOR_DETECTOR_H
