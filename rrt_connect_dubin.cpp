#include "rrt_connect_dubin.h"

QPair<double, double> RRT_Connect_Dubin(painter_widget *paintArea, QProgressBar *progress_bar, int max_iter, bool dynamic_drawing, bool stop_when_found, double step){
    //Variable Transfer
    Point start = paintArea->get_start();
    Point end = paintArea->get_end();
    QVector<QRect> obstacles = paintArea->get_obstacles();
    double time = -1;
    QElapsedTimer timer;
    Tree *T1;
    Tree *T2;
    QPair<int, Point>closest_pair;
    QPair<int, Point> np;
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> best_path = QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>>(INFINITY, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>());
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> cur_path1;
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> cur_path2;
    QPair<QPair<int, Point>, QPair<int, Point>> edge;
    QPair<double, QVector<Point>> mid;
    Robot robot(12, 15, M_PI/4);

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
        np = T1->grow_tree_dubin(paintArea, step, obstacles, robot, "1Green", QColor(0, 255, 0, 255));
        closest_pair = T2->find_nearest(np.second);
        if(np.first != -1 && distance(np.second, closest_pair.second) <= 10){
            //paintArea->clear_color("2Red");
            //paintArea->clear_color("2Blue");
            //paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), np, closest_pair.second );
            mid = connect(paintArea, np, closest_pair, T1, T2, robot, obstacles);
            if(mid.first == -1){
                continue;
            }
            else{
                //paintArea->add_path_to_draw("2Red", QColor(255, 0, 0, 255), np.first, mid.second);
            }
            time = timer.elapsed();
            cur_path1 = T1->retrace_dubin(np, robot);
            cur_path2 = T2->retrace_dubin(closest_pair, robot);
            if(cur_path1.first + cur_path2.first < best_path.first){
                paintArea->clear_color("2Red");
                paintArea->clear_color("2Blue");
                best_path.first = cur_path1.first + cur_path2.first;
                best_path.second = cur_path1.second;
                best_path.second.append(cur_path2.second);
                foreach(edge, cur_path1.second){
                    paintArea->switch_path_color("1Green", "2Red", QColor(255, 0, 0, 255), edge.first.first, edge.second.second);
                }
                foreach(edge, cur_path2.second){
                    paintArea->switch_path_color("1Green", "2Blue", QColor(0, 0, 255, 255), edge.first.first, edge.second.second);
                }
            }
            if(stop_when_found){
                return QPair<double, double>(best_path.first, time);
            }
        }
        np = T2->grow_tree_dubin(paintArea, step, obstacles, robot, "1Green", QColor(0, 255, 0, 255));
        closest_pair = T1->find_nearest(np.second);
        if(np.first != -1 && distance(np.second, closest_pair.second) <= 10 && is_valid(paintArea, np.second, closest_pair.second, obstacles)){
            //paintArea->clear_color("2Red");
            //paintArea->clear_color("2Blue");
            //paintArea->add_to_draw("2Blue", QColor(0, 0, 255, 255), np, closest_pair.second);
            mid = connect(paintArea, np, closest_pair, T2, T1, robot, obstacles);
            if(mid.first == -1){
                continue;
            }
            else{
                //paintArea->add_path_to_draw("2Red", QColor(255, 0, 0, 255), np.first, mid.second);
            }
            time = timer.elapsed();
            cur_path1 = T1->retrace_dubin(closest_pair, robot);
            cur_path2 = T2->retrace_dubin(np, robot);
            if(cur_path1.first + cur_path2.first < best_path.first){
                paintArea->clear_color("2Red");
                paintArea->clear_color("2Blue");
                best_path.first = cur_path1.first + cur_path2.first;
                best_path.second = cur_path1.second;
                best_path.second.append(cur_path2.second);
                foreach(edge, cur_path1.second){
                    paintArea->switch_path_color("1Green", "2Blue", QColor(0, 0, 255, 255), edge.first.first, edge.second.second);
                }
                foreach(edge, cur_path2.second){
                    paintArea->switch_path_color("1Green", "2Red", QColor(255, 0, 0, 255), edge.first.first, edge.second.second);
                }
            }
            if(stop_when_found){
                return QPair<double, double>(best_path.first, time);
            }
        }
    }
    if(time == -1){
        time = timer.elapsed();
    }
    return QPair<double, double>(best_path.first, time);
}
