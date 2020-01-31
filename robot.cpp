#include "robot.h"

Robot::Robot()
{

}

Robot::Robot(double si, double Li, double max_anglei)
{
    this->L = Li;
    this->s = si;
    this->max_angle = max_anglei;
    this->rho = L/qTan(max_anglei);
}

double dist(QPointF node1, QPointF node2){
    return qSqrt(qPow(node1.x() - node2.x(), 2) + qPow(node1.y() - node2.y(), 2));
}

QPair<QPair<QString, QVector<double>>, double> Robot::min_dist(Point node0, Point node1){
    Point node0bar = node0.NormByRho(this->rho);
    Point node1bar = node1.NormByRho(this->rho);
    QPair<QVector<double>, double> LSL = min_LSL(node0bar, node1bar);
    QPair<QVector<double>, double> RSL = min_RSL(node0bar, node1bar);
    QPair<QVector<double>, double> RSR = min_RSR(node0bar, node1bar);
    QPair<QVector<double>, double> LSR = min_LSR(node0bar, node1bar);
    QPair<QVector<double>, double> RLR = min_RLR(node0bar, node1bar);
    QPair<QVector<double>, double> LRL = min_LRL(node0bar, node1bar);
    double min = std::min({LSL.second, RSL.second, RSR.second, LSR.second, RLR.second, LRL.second});
    if (min == LSL.second){
        return QPair<QPair<QString, QVector<double>>, double>(QPair<QString, QVector<double>>("LSL", LSL.first), min*this->rho);
    }
    if (min == RSL.second){
        return QPair<QPair<QString, QVector<double>>, double>(QPair<QString, QVector<double>>("RSL", RSL.first), min*this->rho);
    }
    if (min == RSR.second){
        return QPair<QPair<QString, QVector<double>>, double>(QPair<QString, QVector<double>>("RSR", RSR.first), min*this->rho);
    }
    if (min == LSR.second){
        return QPair<QPair<QString, QVector<double>>, double>(QPair<QString, QVector<double>>("LSR", LSR.first), min*this->rho);
    }
    if (min == RLR.second){
        return QPair<QPair<QString, QVector<double>>, double>(QPair<QString, QVector<double>>("RLR", RLR.first), min*this->rho);
    }
    if (min == LRL.second){
        return QPair<QPair<QString, QVector<double>>, double>(QPair<QString, QVector<double>>("LRL", LRL.first), min*this->rho);
    }
}

QPair<QVector<double>, double> Robot::min_LSL(Point node0, Point node1){
    double tx = node1.x() - node0.x() + qSin(node0.theta()) - qSin(node1.theta());
    double ty = node1.y() - node0.y() - qCos(node0.theta()) + qCos(node1.theta());
    double phi = qAtan(ty/tx);
    double d = distance(node0, node1);
    double A;
    if (tx > 0){
        A = phi;
    }
    else if (tx < 0){
        A = phi + M_PI;
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({0, 0, 0}), INFINITY);
    }
    double dbar = qPow(d, 2) + 2 - 2*qCos(node0.theta() - node1.theta()) + 2*(node1.x() - node0.x())*
            (qSin(node0.theta()) - qSin(node1.theta())) + 2*(node1.y() - node0.y())*(qCos(node1.theta()) - qCos(node0.theta()));
    return QPair<QVector<double>, double>(QVector<double>({fmod((A - node0.theta()),(2*M_PI)),
                                                           qSqrt(dbar)*this->rho,
                                                           fmod((node1.theta() - A),(2*M_PI))}),
                                          qSqrt(dbar) + fabs(this->rho*fmod((A - node0.theta()),(2*M_PI))) + fabs(this->rho*fmod((node1.theta() - A),(2*M_PI))));
}

QPair<QVector<double>, double> Robot::min_RSL(Point node0, Point node1){
    double tx = node1.x() - node0.x() - qSin(node0.theta()) - qSin(node1.theta());
    double ty = node1.y() - node0.y() + qCos(node0.theta()) + qCos(node1.theta());
    double phi = qAtan(tx/ty);
    double d = distance(node0, node1);
    double A;
    if (ty > 0){
        A = phi + M_PI;
    }
    else if (ty < 0){
        A = phi;
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({0, 0, 0}), INFINITY);
    }
    double dbar = qPow(d, 2) - 2 + 2*qCos(node0.theta() - node1.theta()) - 2*(node1.x() - node0.x())*
            (qSin(node0.theta()) + qSin(node1.theta())) + 2*(node1.y() - node0.y())*(qCos(node1.theta()) + qCos(node0.theta()));
    double B = qAtan(qSqrt(dbar)/2);
    return QPair<QVector<double>, double>(QVector<double>({fmod((node0.theta() - B + A),(2*M_PI)),
                                                           qSqrt(dbar)*this->rho,
                                                          fmod((node1.theta() - B + A),(2*M_PI))}),
                                          qSqrt(dbar) + fabs(this->rho*fmod((node0.theta() - B + A),(2*M_PI))) + fabs(this->rho*fmod((node1.theta() - B + A),(2*M_PI))));
}

QPair<QVector<double>, double> Robot::min_RSR(Point node0, Point node1){
    double tx = node1.x() - node0.x() - qSin(node0.theta()) + qSin(node1.theta());
    double ty = node1.y() - node0.y() + qCos(node0.theta()) - qCos(node1.theta());
    double phi = qAtan(ty/tx);
    double d = distance(node0, node1);
    double A;
    if (tx > 0){
        A = phi;
    }
    else if (tx < 0){
        A = phi + M_PI;
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({0, 0, 0}), INFINITY);
    }
    double dbar = qPow(d, 2) + 2 - 2*qCos(node0.theta() - node1.theta()) + 2*(node1.x() - node0.x())*
            (qSin(node1.theta()) - qSin(node0.theta())) + 2*(node1.y() - node0.y())*(qCos(node0.theta()) - qCos(node1.theta()));
    return QPair<QVector<double>, double>(QVector<double>({fmod((node0.theta() - A),(2*M_PI)),
                                                           qSqrt(dbar)*this->rho,
                                                          fmod((A - node1.theta()),(2*M_PI))}),
                                          qSqrt(dbar) + fabs(this->rho*fmod((node0.theta() - A),(2*M_PI))) + fabs(this->rho*fmod((A - node1.theta()),(2*M_PI))));
}
QPair<QVector<double>, double> Robot::min_LSR(Point node0, Point node1){
    double tx = node1.x() - node0.x() + qSin(node0.theta()) + qSin(node1.theta());
    double ty = node1.y() - node0.y() - qCos(node0.theta()) - qCos(node1.theta());
    double phi = qAtan(-tx/ty);
    double d = distance(node0, node1);
    double A;
    if (ty > 0){
        A = phi + M_PI;
    }
    else if (ty < 0){
        A = phi;
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({0, 0, 0}), INFINITY);
    }
    double dbar = qPow(d, 2) - 2 + 2*qCos(node0.theta() - node1.theta()) + 2*(node1.x() - node0.x())*
            (qSin(node0.theta()) + qSin(node1.theta())) - 2*(node1.y() - node0.y())*(qCos(node1.theta()) + qCos(node0.theta()));
    double B = qAtan(qSqrt(dbar)/2);
    return QPair<QVector<double>, double>(QVector<double>({fmod((A - B - node0.theta()),(2*M_PI)),
                                                           qSqrt(dbar)*this->rho,
                                                          fmod((A - B - node1.theta()),(2*M_PI))}),
                                          qSqrt(dbar) + fabs(this->rho*fmod((A - B - node0.theta()),(2*M_PI))) + fabs(this->rho*fmod((A - B - node1.theta()),(2*M_PI))));
}

QPair<QVector<double>, double> Robot::min_LRL(Point node0, Point node1){
    double tx = node1.x() - node0.x() + qSin(node0.theta()) - qSin(node1.theta());
    double ty = node1.y() - node0.y() - qCos(node0.theta()) + qCos(node1.theta());
    double phi = qAtan(ty/tx);
    double d = distance(node0, node1);
    double A;
    if (tx > 0){
        A = phi;
    }
    else if (tx < 0){
        A = phi + M_PI;
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({0, 0, 0}), INFINITY);
    }
    double dbar = 1/8*(6 - qPow(d, 2) - 2*qCos(node0.theta() - node1.theta()) - 2*(node1.x() - node0.x())*
            (qSin(node0.theta()) - qSin(node1.theta())) + 2*(node1.y() - node0.y())*(qCos(node1.theta()) - qCos(node0.theta())));
    double p1 = qAcos(dbar);
    double t1 = fmod((-node0.theta() + A + p1/2),(2*M_PI));
    double q1 = fmod((node1.theta() - A + p1/2),(2*M_PI));
    double p2 = 2*M_PI - qAcos(dbar);
    double t2 = fmod((-node0.theta() + A + p2/2),(2*M_PI));
    double q2 = fmod((node1.theta() - A + p2/2),(2*M_PI));
    if (fabs(this->rho*p1) + fabs(this->rho*t1) + fabs(this->rho*q1) < fabs(this->rho*p2) + fabs(this->rho*t2) + fabs(this->rho*q2)){
        return QPair<QVector<double>, double>(QVector<double>({t1, p1, q1}), fabs(this->rho*p1) + fabs(this->rho*t1) + fabs(this->rho*q1));
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({t2, p2, q2}), fabs(this->rho*p2) + fabs(this->rho*t2) + fabs(this->rho*q2));
    }
}

QPair<QVector<double>, double> Robot::min_RLR(Point node0, Point node1){
    double tx = node1.x() - node0.x() - qSin(node0.theta()) + qSin(node1.theta());
    double ty = node1.y() - node0.y() + qCos(node0.theta()) - qCos(node1.theta());
    double phi = qAtan(ty/tx);
    double d = distance(node0, node1);
    double A;
    if (tx > 0){
        A = phi;
    }
    else if (tx < 0){
        A = phi + M_PI;
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({0, 0, 0}), INFINITY);
    }
    double dbar = 1/8*(6 - qPow(d, 2) + 2*qCos(node0.theta() - node1.theta()) + 2*(node1.x() - node0.x())*
            (qSin(node0.theta()) - qSin(node1.theta())) - 2*(node1.y() - node0.y())*(qCos(node1.theta()) - qCos(node0.theta())));
    double p1 = qAcos(dbar);
    double t1 = fmod((node0.theta() - A + p1/2),(2*M_PI));
    double q1 = fmod((-node1.theta() + A + p1/2),(2*M_PI));
    double p2 = 2*M_PI - qAcos(dbar);
    double t2 = fmod((node0.theta() - A + p2/2),(2*M_PI));
    double q2 = fmod((-node1.theta() + A + p2/2),(2*M_PI));
    if (fabs(this->rho*p1) + fabs(this->rho*t1) + fabs(this->rho*q1) < fabs(this->rho*p2) + fabs(this->rho*t2) + fabs(this->rho*q2)){
        return QPair<QVector<double>, double>(QVector<double>({t1, p1, q1}), fabs(this->rho*p1) + fabs(this->rho*t1) + fabs(this->rho*q1));
    }
    else{
        return QPair<QVector<double>, double>(QVector<double>({t2, p2, q2}), fabs(this->rho*p2) + fabs(this->rho*t2) + fabs(this->rho*q2));
    }
}

QVector<Point> Robot::gen_L(Point start, double delta_theta, int nb_points){
    QVector<Point> out;
    out.append(start);
    double h = fabs((delta_theta)/(nb_points*this->s/this->L * qTan(-this->max_angle)));
    Point cp = start;
    for(int i = 0; i < nb_points; ++i){
        cp.setX(cp.x() + h*this->s * qCos(cp.theta()));
        cp.setY(cp.y() + h*this->s * qCos(cp.theta()));
        cp.setT(cp.theta() + h*this->s/this->L * qTan(-this->max_angle));
        out.append(cp);
    }
    return out;
}

QVector<Point> Robot::gen_R(Point start, double delta_theta, int nb_points){
    QVector<Point> out;
    out.append(start);
    double h = fabs((delta_theta)/(nb_points*this->s/this->L * qTan(this->max_angle)));
    Point cp = start;
    for(int i = 0; i < nb_points; ++i){
        cp.setX(cp.x() + h*this->s * qCos(cp.theta()));
        cp.setY(cp.y() + h*this->s * qCos(cp.theta()));
        cp.setT(cp.theta() + h*this->s/this->L * qTan(this->max_angle));
        out.append(cp);
    }
    return out;
}

QVector<Point> Robot::gen_S(Point start, double delta_d){
    return QVector<Point>({start, Point(start.x() + delta_d*qCos(start.theta()), start.x() + delta_d*qSin(start.theta()), start.theta())});
}
