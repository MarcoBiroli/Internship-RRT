#include "rrt_connect.h"

QPair<double, double> RRT_Connect(painter_widget *paintArea, QProgressBar *progress_bar, int max_iter, bool dynamic_drawing, bool stop_when_found, double step){
    //Variable Transfer
    Point start = paintArea->get_start();
    Point end = paintArea->get_end();
    QVector<QRect> obstacles = paintArea->get_obstacles();
    double time = -1;
    QElapsedTimer timer;
    Tree *T1;
    Tree *T2;
    Point cur_point;
    QPair<int, Point>closest_pair;
    int closest_id;
    Point closest;
    Point np;
    int np_id;
    int point_id1;
    Point point_1;
    int point_id2;
    Point point_2;
    QPair<double , QPair<QVector<QPair<Point, Point>>, QVector<QPair<Point, Point>>>> best_path = QPair<double ,
            QPair<QVector<QPair<Point, Point>>, QVector<QPair<Point, Point>>>>
            (INFINITY, QPair<QVector<QPair<Point, Point>>, QVector<QPair<Point, Point>>>
             (QVector<QPair<Point, Point>>(), QVector<QPair<Point, Point>>()));
    QPair<double , QPair<QVector<QPair<Point, Point>>, QVector<QPair<Point, Point>>>> cur_path;
    QPair<Point, Point> edge;

    // Checks
    if (start.isNull()){
        return QPair<double, double>(-1, -1);
    }
    if (end.isNull()){
        return QPair<double, double>(-1, -1);
    }
    if (step < 1){
        return QPair<double, double>(-1, -1);
    }

    //Program
    T1 = new Tree(start);
    T2 = new Tree(end);
    timer.start();
    for (int i = 0; i < max_iter; i++)
    {
        if (dynamic_drawing){
            paintArea->repaint();
            progress_bar->setValue((int)(100*i/max_iter));
            QCoreApplication::processEvents();
        }
        cur_point = rand_point(paintArea->width(), paintArea->height());
        closest_pair = T1->find_nearest(cur_point);
        closest_id = closest_pair.first;
        closest = closest_pair.second;
        np = build(closest, cur_point, step);
        if (is_valid(paintArea, closest, np, obstacles)){
            np_id = T1->addNode(np);
            T1->addEdge(closest_id, np_id);
            paintArea->add_to_draw("1Green", QColor(0, 255, 0, 255), np, closest);
            closest_pair = T2->find_nearest(np);
            if(distance(np, closest_pair.second) <= 10 && is_valid(paintArea, np, closest_pair.second, obstacles)){
                //paintArea->clear_color("2Red");
                //paintArea->clear_color("2Blue");
                //paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), np, closest_pair.second );
                time = timer.elapsed();
                cur_path.first = 1;
                cur_path.second.first.clear();
                cur_path.second.first.append(QPair<Point, Point>(np, closest_pair.second));
                cur_path.second.second.clear();
                point_id1 = np_id;
                point_1 = np;
                point_id2 = -1;
                point_2 = Point();
                while(point_1 != start){
                    cur_path.first ++;
                    point_id2 = T1->backward_links[point_id1][0];
                    point_2 = T1->nodes[point_id2];
                    //paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), point_1, point_2);
                    cur_path.second.first.append(QPair<Point, Point>(point_1, point_2));
                    point_1 = point_2;
                    point_id1 = point_id2;
                }
                point_id1 = closest_pair.first;
                point_1 = closest_pair.second;
                point_id2 = -1;
                point_2 = Point();
                while(point_1 != end){
                    cur_path.first ++;
                    point_id2 = T2->backward_links[point_id1][0];
                    point_2 = T2->nodes[point_id2];
                    //paintArea->add_to_draw("2Blue", QColor(0, 0, 255, 255), point_1, point_2);
                    cur_path.second.second.append(QPair<Point, Point>(point_1, point_2));
                    point_1 = point_2;
                    point_id1 = point_id2;
                }
                cur_path.first *= step;
                if(cur_path.first < best_path.first){
                    paintArea->clear_color("2Red");
                    paintArea->clear_color("2Blue");
                    best_path.first = cur_path.first;
                    best_path.second.first = cur_path.second.first;
                    best_path.second.second = cur_path.second.second;
                    foreach(edge, best_path.second.first){
                        paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), edge.first, edge.second);
                    }
                    foreach(edge, best_path.second.second){
                        paintArea->add_to_draw("2Blue", QColor(0, 0, 255, 255), edge.first, edge.second);
                    }
                }
                if(stop_when_found){
                    return QPair<double, double>(best_path.first, time);
                }
            }
        }
        cur_point = rand_point(paintArea->width(), paintArea->height());
        closest_pair = T2->find_nearest(cur_point);
        closest_id = closest_pair.first;
        closest = closest_pair.second;
        np = build(closest, cur_point, step);
        if (is_valid(paintArea, closest, np, obstacles)){
            np_id = T2->addNode(np);
            T2->addEdge(closest_id, np_id);
            paintArea->add_to_draw("1GReen", QColor(0, 255, 0, 255), np, closest);
            closest_pair = T1->find_nearest(np);
            if(distance(np, closest_pair.second) <= 10 && is_valid(paintArea, np, closest_pair.second, obstacles)){
                //paintArea->clear_color("2Red");
                //paintArea->clear_color("2Blue");
                //paintArea->add_to_draw("2Blue", QColor(0, 0, 255, 255), np, closest_pair.second);
                time = timer.elapsed();
                cur_path.first = 1;
                cur_path.second.first.clear();
                cur_path.second.first.append(QPair<Point, Point>(np, closest_pair.second));
                cur_path.second.second.clear();
                point_id1 = np_id;
                point_1 = np;
                point_id2 = -1;
                point_2 = Point();
                while(point_1 != end){
                    cur_path.first ++;
                    point_id2 = T2->backward_links[point_id1][0];
                    point_2 = T2->nodes[point_id2];
                    //paintArea->add_to_draw("2Blue", QColor(0,0,255,255), point_1, point_2);
                    cur_path.second.first.append(QPair<Point, Point>(point_1, point_2));
                    point_1 = point_2;
                    point_id1 = point_id2;
                }
                point_id1 = closest_pair.first;
                point_1 = closest_pair.second;
                point_id2 = -1;
                point_2 = Point();
                while(point_1 != start){
                    cur_path.first ++;
                    point_id2 = T1->backward_links[point_id1][0];
                    point_2 = T1->nodes[point_id2];
                    //paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), point_1, point_2);
                    cur_path.second.second.append(QPair<Point, Point>(point_1, point_2));
                    point_1 = point_2;
                    point_id1 = point_id2;
                }
                cur_path.first *= step;
                if(cur_path.first < best_path.first){
                    paintArea->clear_color("2Red");
                    paintArea->clear_color("2Blue");
                    best_path = cur_path;
                    foreach(edge, best_path.second.first){
                        paintArea->add_to_draw("2Blue", QColor(0, 0, 255, 255), edge.first, edge.second);
                    }
                    foreach(edge, best_path.second.second){
                        paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), edge.first, edge.second);
                    }
                }
                if(stop_when_found){
                    return QPair<double, double>(best_path.first, time);
                }
            }
        }
    }
    if(time == -1){
        time = timer.elapsed();
    }
    return QPair<double, double>(best_path.first, time);
}
