//
// Created by alumno on 20/12/22.
//

#ifndef FORCEFIELD_Generic_Object_H
#define FORCEFIELD_Generic_Object_H

#include <Eigen/Dense>
#include <YoloObjects.h>
#include <JointMotorSimple.h>
#include <OmniRobot.h>
#include "door_detector.h"
#include "Door.h"


class Generic_Object {
private:
    int id;
    int type;
    int left;
    int top;
    int right;
    int bot;
    float score;
    float depth;
    float x;
    float y;
    float z;

public:
    Generic_Object();
    Generic_Object(const Door &door);
    Generic_Object(const RoboCompYoloObjects:: TBox &tbox);

    void save_doors(const std::vector<Door> &doors, std::vector<Generic_Object> &generic_object_vector );
    static std::vector<Generic_Object> save_objects(const std::vector<RoboCompYoloObjects::TBox> &objs);

};


#endif //FORCEFIELD_Generic_Object_H
