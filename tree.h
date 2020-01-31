#ifndef TREE_H
#define TREE_H

#include <QPair>
#include <QtCore>
#include <QMap>
#include <QtGui>
#include <QVector>
#include "point.h"
#include "node.h"
#include "common.h"
#include "painter_widget.h"
#include "path.h"
#include "robot.h"

class Tree{
public:
    Tree(Point iroot);
    Tree(Point root, int max_nodes, double x_max, double y_max);
    int addNode(Point node);
    void addEdge(int node1, int node2);
    QPair<int, Point> find_nearest(Point node);
    QMap<int, QVector<int>> forward_links;
    QMap<int, QVector<int>> backward_links;
    Point root;
    int node_id = 0;
    QMap<int, Point> nodes;
    QMap<int, double> distances;
    QVector<QPair<int, Point>> get_close(Point node, double rad);
    QPair<Point, Point> remove_parent_edge(QPair<int, Point> node);
    QPair<int, Point> choose_parent(painter_widget *paintArea, Point node, QVector<QPair<int, Point>> candidates, QVector<QRect> obstacles);
    void rewire(painter_widget *paintArea, QVector<QPair<int, Point>> close_points, QPair<int, Point> new_point, QVector<QRect> obstacles);
    bool rewire_fn(painter_widget *paintArea, QVector<QPair<int, Point>> close_points, QPair<int, Point> new_point, QVector<QRect> obstacles);
    bool IsOnlyChild(QPair<int, Point> node);
    QVector<QPair<Point, Point>> remove_node(int node);
    QVector<QPair<Point, Point>> remove_parent(QPair<int, Point> node);
    bool force_delete(painter_widget *paintArea);
    QPair<int, QVector<Point>> find_nearest_dubin(Point node, Robot robot, double step);
    QPair<int, Point> grow_tree(painter_widget* paintArea, double step, QVector<QRect> obstacles, QString scolor, QColor color);
    QPair<int, Point> grow_tree_dubin(painter_widget* paintArea, double step, QVector<QRect> obstacles, Robot robot, QString scolor, QColor color);
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> retrace(QPair<int, Point> fp);
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> retrace_dubin(QPair<int, Point> fp, Robot robot);

private:
    double max_nodes;
    double x_max;
    double y_max;
    bool from_paint_area = false;
};

#endif // TREE_H
