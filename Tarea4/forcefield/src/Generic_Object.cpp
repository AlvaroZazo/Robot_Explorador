//
// Created by alumno on 20/12/22.
//

#include "Generic_Object.h"

Generic_Object::Generic_Object(const RoboCompYoloObjects::TBox &tbox){
    id=tbox.id;
    type=tbox.type;
    left=tbox.left;
    top=tbox.top;
    right=tbox.right;
    bot=tbox.bot;
    score=tbox.score;
    depth=tbox.depth;
    x=tbox.x;
    y=tbox.y;
    z=tbox.z;
}

Generic_Object::Generic_Object(const Door &door){
    id=81;
    x=door.center.x();
    y=door.center.y();
    z=0;
}

void save_doors(const std::vector<Door> &doors,std::vector<Generic_Object> &generic_object_vector ){
    for(const auto d:doors){
        generic_object_vector.push_back(Generic_Object(d));
    }
}
std::vector<Generic_Object> save_objects(const std::vector<RoboCompYoloObjects::TBox> &objs){
    std::vector<Generic_Object> objects;
    for(const auto d:objs){
        objects.push_back(Generic_Object(d));
    }
    return objects;
}