//
// Created by alumno on 7/12/22.
//

#include "door_detector.h"



std::vector<Door_detector::Door> Door_detector::door_detector(const vector<Eigen::Vector2f> &line, float door_dynamic_threshold) {
    std::vector<float> derivaties(line.size() - 1);
    for (auto &&[i, d]: line | iter::sliding_window(2) | iter::enumerate)
        derivaties[i] = d[1].norm() - d[0].norm();

    std::vector<std::tuple<int, bool>> peaks;
    for (auto &&[i, d]: derivaties | iter::enumerate)
        if (d > door_dynamic_threshold)
            peaks.push_back(std::make_tuple(i, true));
        else
            peaks.push_back(std::make_tuple(i, false));

    std::vector<Door> doors;

    for (auto &&p: peaks | iter::combinations_with_replacement(2)) {
        auto &[p1, pos1] = p[0];
        auto &[p2, pos2] = p[1];
        Eigen::Vector2f v1 = line[p1];
        Eigen::Vector2f v2 = line[p2];

        if (((pos1 and not pos2) or (pos2 and not pos1)) and (v1 - v2).norm() < 1000 and (v1 - v2).norm() > 600){
            Door door;
            door.peak_right=v1;
            door.peak_left=v2;
            door.center=(v1 + v2) / 2;
            doors.push_back(door);
        }


    }
    return doors;
}






void Door_detector::draw_doors(const std::vector<Door> &doors) {
    static std::vector<QGraphicsItem *> items;
    for (const auto &i: items)
        viewer->scene.removeItem(i);
    items.clear();

    for (const auto &d: doors) {
        auto item = viewer->scene.addEllipse(-100, -100, 200, 2 * 200, QPen(QColor("yellow")),
                                             QBrush(QColor("yellow")));
        // Coordenadas de la puerta
        item->setPos(d.peak_left(0), d.peak_right(0));
        items.push_back(item);
    }

}