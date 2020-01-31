#ifndef PAINTER_WIDGET_H
#define PAINTER_WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include <QMap>
#include "point.h"

namespace Ui {
class painter_widget;
}

class painter_widget : public QWidget
{
    Q_OBJECT
public:
    painter_widget(QWidget *parent = nullptr);
    bool draw;
    Point get_start();
    Point get_end();
    QVector<QRect> get_obstacles();
    QPen linePen;
    void add_to_draw(QString scolor, QColor color, Point node1, Point node2);
    void add_path_to_draw(QString scolor, QColor color, int parent, QVector<Point> path);
    void reset();
    void delete_to_draw(QString scolor, QVector<QPair<Point, Point>> deleted_edges);
    QRect in_obstacle(Point point);
    void clear_color(QString color);
    void switch_path_color(QString prev_scolor, QString scolor, QColor color, int parent, Point child);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Point start;
    Point end;
    Point click_point;
    QPen startPen;
    QPen endPen;
    QPen obstaclePen;
    QPen pathPen;
    bool has_moved = false;
    QVector<QRect> obstacles;
    QMap<QString, QPair<QColor, QVector<QPair<Point, Point>>>> to_draw;
    QMap<QString, QPair<QColor, QMap<int, QVector<QVector<Point>>>>> paths_to_draw;
};

#endif // PAINTER_WIDGET_H
