#ifndef POINT_H
#define POINT_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>

class Point
{
public:
    Point();
    Point(double xi, double yi);
    Point(double xi, double yi, double thetai);
    double x() const;
    double y() const;
    double theta() const;
    void setX(double sx);
    void setY(double sy);
    void setT(double st);
    QVector<QVector<Point>> generate_dubin(int nb, double s, double L, double max_turn, double step);
    QVector<Point> rk4_dubin(int nb, double u_s, double u_phi, double step, double L);
    bool isNull() const{
        return (this->x() == 0 && this->y() == 0 && this->theta() == 0);
    }
    QPointF node;
    Point NormByRho(double rho);
    bool operator == (const Point &other) const{
        return (this->x() == other.x() && this->y() == other.y() && this->theta() == other.theta());
    }
    bool operator != (const Point &other) const{
        return !(this->x() == other.x() && this->y() == other.y() && this->theta() == other.theta());
    }
    /*
    bool operator < (const Point &other) const{
        return (this->x() < other.x() && this->y() < other.y());
    }
    */

private:
    double px = 0;
    double py = 0;
    double ptheta = 0;
};

double distance(Point A, Point B);

#endif // POINT_H
