#include "painter_widget.h"

painter_widget::painter_widget(QWidget *parent) : QWidget(parent)
{
    startPen.setColor(QColor(0, 0, 255, 255));
    startPen.setWidth(10);

    endPen.setColor(QColor(255, 0, 0, 255));
    endPen.setWidth(10);

    linePen.setColor(QColor(0, 255, 0, 255));
    linePen.setWidth(1);

    obstaclePen.setColor((QColor(255, 255, 255, 255)));
    obstaclePen.setWidth(1);

    pathPen.setColor(QColor(255, 0, 0, 255));
    pathPen.setWidth(1);
    this->setAutoFillBackground(true);

    /*
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start();
    */

}


void painter_widget::paintEvent(QPaintEvent * /* event */)
{

    QPainter painter(this);

    painter.setPen(startPen);
    painter.drawPoint(start.node);

    painter.setPen(endPen);
    painter.drawPoint(end.node);

    painter.setPen(obstaclePen);
    QRect cur_rect;
    foreach (cur_rect, obstacles) {
        painter.fillRect(cur_rect, QColor(255,255,255,255));
    }

    QPair<Point, Point> edge;
    Point point1;
    Point point2;
    QMap<QString, QPair<QColor, QVector<QPair<Point, Point>>>>::iterator i;
    for(i = to_draw.begin(); i != to_draw.end(); ++i){
        pathPen.setColor(i.value().first);
        painter.setPen(pathPen);
        foreach(edge, i.value().second){
            point1 = edge.first;
            point2 = edge.second;
            painter.drawLine(point1.node, point2.node);
        }
    }
    QMap<QString, QPair<QColor, QMap<int, QVector<QVector<Point>>>>>::iterator j;
    for(j = paths_to_draw.begin(); j != paths_to_draw.end(); j++){
        pathPen.setColor(j.value().first);
        painter.setPen(pathPen);
        QMap<int, QVector<QVector<Point>>>::iterator k;
        for(k = j.value().second.begin(); k != j.value().second.end(); ++k){
            QVector<Point> path;
            foreach (path, k.value()) {
                for(int l = 1; l < path.length(); ++l){
                    painter.drawLine(path[l-1].node, path[l].node);
                }
            }
        }
    }
}

void painter_widget::mousePressEvent(QMouseEvent *event)
{
    /*
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
    */
    has_moved = false;
    click_point.setX(event->x());
    click_point.setY(event->y());
}

void painter_widget::mouseMoveEvent(QMouseEvent *event)
{
    /*
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
    */
    has_moved = true;
}

void painter_widget::mouseReleaseEvent(QMouseEvent *event)
{
    /*
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
    */
    if (has_moved == false){
        QRect container = this->in_obstacle(click_point);
        if (!container.isNull()){
            this->obstacles.remove(this->obstacles.indexOf(container));
        }
        else{
            if (event->button() == Qt::LeftButton){
                start = click_point;
            }
            if (event->button() == Qt::RightButton){
                end = click_point;
            }
        }
    }
    else{
        int dx = event->x() - click_point.x();
        int dy = event->y() - click_point.y();
        obstacles.append(QRect(click_point.x(), click_point.y(), dx, dy));
    }
    //this->to_draw["2Delete"].second.clear();
    this->update();
}

QRect painter_widget::in_obstacle(Point point){
    QRect obstacle;
    foreach (obstacle, this->obstacles) {
        if(obstacle.contains(point.x(), point.y())){
            return obstacle;
        }
    }
    return QRect();
}

Point painter_widget::get_start(){
    return this->start;
}

Point painter_widget::get_end(){
    return this->end;
}

QVector<QRect> painter_widget::get_obstacles(){
    return this->obstacles;
}

void painter_widget::reset(){
    to_draw.clear();
    paths_to_draw.clear();
    this->update();
}

void painter_widget::add_to_draw(QString scolor, QColor color, Point node1, Point node2)
{
    to_draw[scolor].first = color;
    to_draw[scolor].second.append(QPair<Point, Point>(node1, node2));
}

void painter_widget::delete_to_draw(QString scolor, QVector<QPair<Point, Point>> deleted_edges){
    QPair<Point, Point> edge;
    foreach(edge, deleted_edges){
        if (to_draw[scolor].second.contains(edge)){
            //this->add_to_draw("2Delete", this->palette().color(this->backgroundRole()), edge.first, edge.second);
            to_draw[scolor].second.remove(to_draw[scolor].second.indexOf(edge));
        }
    }
}

void painter_widget::clear_color(QString color){
    /*
    QPair<Point, Point> edge;
    foreach (edge, this->to_draw[color].second) {
        this->add_to_draw("2Delete", this->palette().color(this->backgroundRole()), edge.first, edge.second);
    }
    */
    this->to_draw[color].second.clear();
    this->paths_to_draw[color].second.clear();
}

void painter_widget::add_path_to_draw(QString scolor, QColor color, int parent, QVector<Point> path){
    this->paths_to_draw[scolor].first = color;
    this->paths_to_draw[scolor].second[parent].append(path);
}

void painter_widget::switch_path_color(QString prev_scolor, QString scolor, QColor color, int parent, Point child){
    QVector<Point> path;
    foreach(path, this->paths_to_draw[prev_scolor].second[parent]){
        if (path.contains(child)){
            this->add_path_to_draw(scolor, color, parent, path);
            break;
        }
    }
}
