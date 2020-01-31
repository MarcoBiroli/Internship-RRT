#include "common_func.h"

QPair<double, QVector<Point>> connect(painter_widget *paintArea, QPair<int, Point> A, QPair<int, Point> B, Tree *T1,
               Tree *T2, Robot robot, QVector<QRect> obstacles){
    QPair<QPair<QString, QVector<double>>, double> connection = robot.min_dist(A.second, B.second);
    QVector<Point> out;
    QPair<int, Point> starting_point = A;
    QVector<Point> part;
    QString type;
    int np_id;
    for(int i = 0; i < 3; i++){
        type = connection.first.first[i];
        if(type == "L"){
            part = robot.gen_L(starting_point.second, connection.first.second[i], 5);
        }
        if(type == "R"){
            part = robot.gen_R(starting_point.second, connection.first.second[i], 5);
        }
        if(type == "S"){
            part = robot.gen_S(starting_point.second, connection.first.second[i]);
        }
        np_id = T1->addNode(part.last());
        if(is_path_valid(paintArea, part, obstacles)){
            T1->addEdge(starting_point.first, np_id);
            paintArea->add_path_to_draw("1Black", QColor(0, 255, 0, 255), starting_point.first, part);
            out.append(part);
            starting_point = QPair<int, Point>(np_id, part.last());
        }
        else{
            return QPair<double, QVector<Point>>(-1, QVector<Point>());
        }
    }
    return QPair<double, QVector<Point>>(connection.second, out);
}
