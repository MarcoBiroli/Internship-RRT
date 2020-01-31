#include "rrt_star_fn.h"

QPair<double, double> RRT_Star_FN(painter_widget *paintArea, QProgressBar *progress_bar, int max_iter, bool dynamic_drawing, bool stop_when_found, double step, int M){


    //-----------------------------------------------------------------------------------
    //Variables Initialization
    bool deleted = false;
    double rad = 50;
    double time = -1;
    Point start = paintArea->get_start();
    Point end = paintArea->get_end();
    QVector<QRect> obstacles = paintArea->get_obstacles();
    QElapsedTimer timer;
    Tree *T1;
    Point cur_point;
    QPair<int, Point> closest_pair;
    int closest_id;
    Point closest;
    Point np;
    int np_id;
    QVector<QPair<int, Point>> close_points;
    QPair<int, Point> parent;
    int point_id1;
    Point point_1;
    int point_id2;
    Point point_2;
    QPair<double , QVector<QPair<Point, Point>>> best_path = QPair<double , QVector<QPair<Point, Point>>>(INFINITY, QVector<QPair<Point, Point>>());
    QPair<double, QVector<QPair<Point, Point>>> cur_path;
    QPair<Point, Point> edge;
    // -------------------------------------------------------------------------------------

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
    timer.start();
    T1 = new Tree(start);
    for (int i = 0; i < max_iter; i++)
    {
        if (dynamic_drawing){
            paintArea->repaint();
            progress_bar->setValue((int)(100*i/max_iter));
            QCoreApplication::processEvents();
        }
        deleted = false;
        cur_point = rand_point(paintArea->width(), paintArea->height());
        closest_pair = T1->find_nearest(cur_point);
        closest_id = closest_pair.first;
        closest = closest_pair.second;
        np = build(closest, cur_point, step);
        if (is_valid(paintArea, closest, np, obstacles)){
            np_id = T1->addNode(np);
            close_points = T1->get_close(np, rad);
            parent = T1->choose_parent(paintArea, np, close_points, obstacles);
            paintArea->add_to_draw("1Green", QColor(0, 255, 0, 255), parent.second, np);
            T1->addEdge(parent.first, np_id);
            if (i < M){
                T1->rewire(paintArea, close_points, QPair<int, Point>(np_id, np), obstacles);
            }
            else{
                deleted = T1->rewire_fn(paintArea, close_points, QPair<int, Point>(np_id, np), obstacles);
                if (!deleted){
                    deleted = T1->force_delete(paintArea);
                    if (!deleted){
                        paintArea->delete_to_draw("1Green", T1->remove_node(np_id));
                    }
                }
            }
            if(distance(np, end) <= 10){
                //paintArea->clear_color("2Red");
                cur_path.first = 0;
                cur_path.second.clear();
                time = timer.elapsed();
                point_id1 = np_id;
                point_1 = np;
                point_id2 = -1;
                point_2 = Point();
                while(point_2 != start){
                    cur_path.first ++;
                    point_id2 = T1->backward_links[point_id1][0];
                    point_2 = T1->nodes[point_id2];
                    //paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), point_1, point_2);
                    cur_path.second.append(QPair<Point, Point>(point_1, point_2));
                    point_1 = point_2;
                    point_id1 = point_id2;
                }
                cur_path.first *= step;
                if (cur_path.first < best_path.first){
                    paintArea->clear_color("2Red");
                    best_path = cur_path;
                    foreach (edge, best_path.second) {
                        paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), edge.first, edge.second);
                    }
                }
                if(stop_when_found){
                    return QPair<double, double>(best_path.first, time);
                }
            }
        }
    }
    if (time == -1){
        time = timer.elapsed();
    }
    return QPair<double, double>(best_path.first, time);
}

