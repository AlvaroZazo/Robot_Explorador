//
// Created by alumno on 14/12/22.
//

#ifndef FORCEFIELD_DOOR_H
#define FORCEFIELD_DOOR_H

#include <cppitertools/combinations_with_replacement.hpp>
#include <cppitertools/sliding_window.hpp>
#include <cppitertools/chunked.hpp>
#include <cppitertools/filter.hpp>
#include <cppitertools/enumerate.hpp>
#include <cppitertools/range.hpp>
#include <Eigen/Dense>

    class Door {

    public:
        Eigen::Vector2f peak_left;
        Eigen::Vector2f peak_right;
        Eigen::Vector2f center;
    };


#endif //FORCEFIELD_DOOR_H
