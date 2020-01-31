#include "rrt_dubin.h"

QPair<double, double> RRT_Dubin(painter_widget *paintArea, QProgressBar *progress_bar, int max_iter, bool dynamic_drawing, bool stop_when_found, double step){
    //Variable Initialization
    Point start = paintArea->get_start();
    Point end = paintArea->get_end();
    QVector<QRect> obstacles = paintArea->get_obstacles();
    double time = -1;
    QElapsedTimer timer;
    Tree *T1;
    QPair<int, Point> np;
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> best_path = QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>>(INFINITY, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>());
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> cur_path;
    QPair<QPair<int, Point>, QPair<int, Point>> edge;
    Robot robot(12, 15, M_PI/2);

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
        np = T1->grow_tree_dubin(paintArea, step, obstacles, robot, "1Green", QColor(0, 255, 0, 255));
        if(np.first != -1 && distance(np.second, end) <= 10){
            //paintArea->clear_color("2Red");
            time = timer.elapsed();
            cur_path = T1->retrace_dubin(np, robot);
            if (cur_path.first < best_path.first){
                paintArea->clear_color("2Red");
                best_path = cur_path;
                foreach(edge, best_path.second){
                    //paintArea->add_to_draw("2Red", QColor(255, 0, 0, 255), edge.first, edge.second);
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

