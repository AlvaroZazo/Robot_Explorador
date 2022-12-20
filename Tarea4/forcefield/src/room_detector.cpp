//
// Created by alumno on 14/12/22.
//

#include "room_detector.h"


std::vector<Room_detector::Room> Room_detector::room_objects(const RoboCompYoloObjects::TObjects objects) {

    for (const auto &o: objects) {
            Room room;
            room.o=v1;
            rooms.push_back(room);
        }


    }
    return rooms;
}

