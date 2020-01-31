#include "point.h"

Point::Point()
{

}

Point::Point(double xi, double yi){
    this->setX(xi);
    this->setY(yi);
}

Point::Point(double xi, double yi, double thetai){
    this->setX(xi);
    this->setY(yi);
    this->setT(thetai);
}

Point Point::NormByRho(double rho){
    return Point(this->x()/rho, this->y()/rho, this->theta());
}

double Point::x() const{
    return this->px;
}

double Point::y() const{
    return this->py;
}

double Point::theta() const{
    return this->ptheta;
}

void Point::setX(double sx){
    this->px = sx;
    this->node.setX(sx);
}

void Point::setY(double sy){
    this->py = sy;
    this->node.setY(sy);
}

void Point::setT(double st){
    this->ptheta = fmod(st, 2*M_PI);
}

QVector<QVector<Point>> Point::generate_dubin(int nb, double s, double L, double max_turn, double step){
    QVector<QVector<Point>> out = QVector<QVector<Point>>(3);
    out[0] = this->rk4_dubin(nb, s, -max_turn, step, L);
    out[1] = this->rk4_dubin(nb, s, 0, step, L);
    out[2] = this->rk4_dubin(nb, s, max_turn, step, L);
    return out;
}

QVector<Point> Point::rk4_dubin(int nb, double u_s, double u_phi, double step, double L){
    /*
     * x'     = u_s * cos ( theta )
     * y'     = u_s * sin ( theta )
     * theta' = u_s / L * tan u_phi
     */
    QVector<Point> out;
    out.append(*this);
    double h = 0.5;
    Point current_point = *this;
    QVector<double> k1 = QVector<double>(3);
    QVector<double> k2 = QVector<double>(3);
    QVector<double> k3 = QVector<double>(3);
    QVector<double> k4 = QVector<double>(3);
    for(int i = 0; i < nb; ++i){
        k1 = {h*u_s*qCos(current_point.theta()), h*u_s*qSin(current_point.theta()), h*u_s*qTan(u_phi)/L};
        k2 = {h*u_s*qCos(current_point.theta() + h/2), h*u_s*qSin(current_point.theta() + h/2), h*u_s*qTan(u_phi + h*k1[2]/2)/L};
        k3 = {h*(u_s*qCos(current_point.theta() + h/2)), h*(u_s*qSin(current_point.theta() + h/2)), h*u_s*qTan(u_phi + h*k2[2]/2)/L};
        k4 = {h*(u_s*qCos(current_point.theta() + h)), h*(u_s*qSin(current_point.theta() + h)), h*u_s*qTan(u_phi + h*k3[2])/L};
        double x = current_point.x() + h*(k1[0] + 2*k2[0] + 2*k3[0] + k4[0])/6;
        double y = current_point.y() + h*(k1[1] + 2*k2[1] + 2*k3[1] + k4[1])/6;
        double theta = current_point.theta() + h*(k1[2] + 2*k2[2] + 2*k3[2] + k4[2])/6;
        current_point = Point(x, y, theta);
        out.append(current_point);
    }
    return out;
}

double distance(Point A, Point B){
    return qSqrt(qPow(A.x() - B.x() ,2) + qPow(A.y() - B.y(),2));
}
