#include "MadgwickQuaternionFilter.h"
#include <cmath>

MadgwickQuaternionFilter::MadgwickQuaternionFilter() :
// MadgwickQuaternionFilter::MadgwickQuaternionFilter(double _deltat):
// deltat(_deltat),
deltat(0.01),
q{{1, 0, 0, 0}} {
    double GyroMeasError = M_PI * (40.0 / 180.0);     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3
    beta = sqrt(3.0 / 4.0) * GyroMeasError;
    double GyroMeasDrift = M_PI * (2.0 / 180.0);      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
    zeta = sqrt(3.0 / 4.0) * GyroMeasDrift;
}

void MadgwickQuaternionFilter::update(double ax, double ay, double az, double gx, double gy, double gz)
{
    double q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];         // short name local variable for readability
    double norm;                                               // vector norm
    double f1, f2, f3;                                         // objetive funcyion elements
    double J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; // objective function Jacobian elements
    double qDot1, qDot2, qDot3, qDot4;
    double hatDot1, hatDot2, hatDot3, hatDot4;
    double gerrx, gerry, gerrz, gbiasx = 0, gbiasy = 0, gbiasz = 0; // gyro bias error

    // Auxiliary variables to avoid repeated arithmetic
    double _halfq1 = 0.5f * q1;
    double _halfq2 = 0.5f * q2;
    double _halfq3 = 0.5f * q3;
    double _halfq4 = 0.5f * q4;
    double _2q1 = 2.0f * q1;
    double _2q2 = 2.0f * q2;
    double _2q3 = 2.0f * q3;
    double _2q4 = 2.0f * q4;
    // double _2q1q3 = 2.0f * q1 * q3;
    // double _2q3q4 = 2.0f * q3 * q4;

    // Normalise accelerometer measurement
    norm = sqrt(ax * ax + ay * ay + az * az);
    if (norm == 0.0f)
        return; // handle NaN
    norm = 1.0f / norm;
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Compute the objective function and Jacobian
    f1 = _2q2 * q4 - _2q1 * q3 - ax;
    f2 = _2q1 * q2 + _2q3 * q4 - ay;
    f3 = 1.0f - _2q2 * q2 - _2q3 * q3 - az;
    J_11or24 = _2q3;
    J_12or23 = _2q4;
    J_13or22 = _2q1;
    J_14or21 = _2q2;
    J_32 = 2.0f * J_14or21;
    J_33 = 2.0f * J_11or24;

    // Compute the gradient (matrix multiplication)
    hatDot1 = J_14or21 * f2 - J_11or24 * f1;
    hatDot2 = J_12or23 * f1 + J_13or22 * f2 - J_32 * f3;
    hatDot3 = J_12or23 * f2 - J_33 * f3 - J_13or22 * f1;
    hatDot4 = J_14or21 * f1 + J_11or24 * f2;

    // Normalize the gradient
    norm = sqrt(hatDot1 * hatDot1 + hatDot2 * hatDot2 + hatDot3 * hatDot3 + hatDot4 * hatDot4);
    hatDot1 /= norm;
    hatDot2 /= norm;
    hatDot3 /= norm;
    hatDot4 /= norm;

    // Compute estimated gyroscope biases
    gerrx = _2q1 * hatDot2 - _2q2 * hatDot1 - _2q3 * hatDot4 + _2q4 * hatDot3;
    gerry = _2q1 * hatDot3 + _2q2 * hatDot4 - _2q3 * hatDot1 - _2q4 * hatDot2;
    gerrz = _2q1 * hatDot4 - _2q2 * hatDot3 + _2q3 * hatDot2 - _2q4 * hatDot1;

    // Compute and remove gyroscope biases
    gbiasx += gerrx * deltat * zeta;
    gbiasy += gerry * deltat * zeta;
    gbiasz += gerrz * deltat * zeta;
    gx -= gbiasx;
    gy -= gbiasy;
    gz -= gbiasz;

    // Compute the quaternion derivative
    qDot1 = -_halfq2 * gx - _halfq3 * gy - _halfq4 * gz;
    qDot2 = _halfq1 * gx + _halfq3 * gz - _halfq4 * gy;
    qDot3 = _halfq1 * gy - _halfq2 * gz + _halfq4 * gx;
    qDot4 = _halfq1 * gz + _halfq2 * gy - _halfq3 * gx;

    // Compute then integrate estimated quaternion derivative
    q1 += (qDot1 - (beta * hatDot1)) * deltat;
    q2 += (qDot2 - (beta * hatDot2)) * deltat;
    q3 += (qDot3 - (beta * hatDot3)) * deltat;
    q4 += (qDot4 - (beta * hatDot4)) * deltat;

    // Normalize the quaternion
    norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4); // normalise quaternion
    norm = 1.0f / norm;
    q[0] = q1 * norm;
    q[1] = q2 * norm;
    q[2] = q3 * norm;
    q[3] = q4 * norm;
}

Eigen::Quaterniond MadgwickQuaternionFilter::getAsQuaternion() {
    return Eigen::Quaterniond(q[0], q[1], q[2], q[3]);
}