#include "common.h"

Point lines_collided(double p0_x, double p0_y, double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y)
{
    Point out = Point(-1, -1);
    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        out.setX(p0_x + (t * s1_x));
        out.setY(p0_y + (t * s1_y));
    }

    return out; // No collision
}

Point rand_point(double width, double height)
{
    double x = QRandomGenerator64::global()->bounded(width);
    double y = QRandomGenerator64::global()->bounded(height);
    double theta = QRandomGenerator64::global()->bounded(2*M_PI);
    Point rp = Point(x, y, theta);
    return rp;
}

Point build(Point start, Point end, double step){
    qreal dx = end.x() - start.x();
    qreal dy = end.y() - start.y();
    qreal norm = qSqrt(qPow(dx,2) + qPow(dy,2));
    return Point(start.x() + step*dx/norm, start.y() + step*dy/norm);
}

bool is_valid(QWidget *paintArea, Point pp, Point np, QVector<QRect> obstacles){
    if (np.x() < 0 || np.y() < 0 || np.x() > paintArea->width() || np.y() > paintArea->height()){
        return false;
    }
    QRect obstacle;
    foreach(obstacle, obstacles){
        int trueleft = fmin(obstacle.left(), obstacle.right());
        int trueright =  fmax(obstacle.left(), obstacle.right());
        int truebottom = fmin(obstacle.bottom(), obstacle.top());
        int truetop = fmax(obstacle.bottom(), obstacle.top());
        Point intersect;
        intersect = lines_collided(trueleft, truebottom, trueright, truetop, pp.x(), pp.y(), np.x(), np.y());
        if (intersect != Point(-1, -1))
        {
            return false;
        }
        intersect = lines_collided(trueleft, truetop, trueright, truebottom, pp.x(), pp.y(), np.x(), np.y());
        if (intersect != Point(-1, -1))
        {
            return false;
        }
        intersect = lines_collided(trueleft, (truebottom + truetop)/2, trueright, (truebottom + truetop)/2, pp.x(), pp.y(), np.x(), np.y());
        if (intersect != Point(-1, -1))
        {
            return false;
        }
        intersect = lines_collided((trueleft + trueright)/2 , truebottom, (trueleft + trueright)/2, truetop, pp.x(), pp.y(), np.x(), np.y());
        if (intersect != Point(-1, -1))
        {
            return false;
        }
        if (np.x() >= trueleft && np.x() <= trueright && np.y() <= truetop && np.y() >= truebottom)
        {
            return false;
        }
    }
    return true;
}

bool is_path_valid(QWidget *paintArea, QVector<Point> path, QVector<QRect> obstacles){
    for(int i = 1; i < path.length(); ++i){
        if(!is_valid(paintArea, path[i-1], path[i], obstacles)){
            return false;
        }
    }
    return true;
}
