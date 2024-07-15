#include "pch.h"
#include "Quaternion.h"

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z
    );
}

Vec3 Quaternion::Rotate(const Vec3& v) const {
    Vec3 quatVec(x, y, z);
    Vec3 uv = quatVec.Cross(v);
    Vec3 uuv = quatVec.Cross(uv);
    uv *= (2.0f * w);
    uuv *= 2.0f;
    return v + uv + uuv;
}

Vec3 Quaternion::ToEulerAngles() const {
    float sinr_cosp = 2 * (w * x + y * z);
    float cosr_cosp = 1 - 2 * (x * x + y * y);
    float roll = atan2(sinr_cosp, cosr_cosp);

    float sinp = 2 * (w * y - z * x);
    float pitch;
    if (fabs(sinp) >= 1)
        pitch = copysign(3.141592 / 2, sinp);
    else
        pitch = asin(sinp);

    float siny_cosp = 2 * (w * z + x * y);
    float cosy_cosp = 1 - 2 * (y * y + z * z);
    float yaw = atan2(siny_cosp, cosy_cosp);

    return Vec3(roll, pitch, yaw);
}

Quaternion QuaternionFromAxisAngle(const Vec3& axis, float angle) {
    float halfAngle = angle * 0.5f;
    float s = sin(halfAngle);
    return Quaternion(axis.x * s, axis.y * s, axis.z * s, cos(halfAngle));
}

Quaternion Quaternion::FromEulerAngles(const Vec3& eulerAngles) {
    float cy = cos(eulerAngles.z * 0.5);
    float sy = sin(eulerAngles.z * 0.5);
    float cp = cos(eulerAngles.y * 0.5);
    float sp = sin(eulerAngles.y * 0.5);
    float cr = cos(eulerAngles.x * 0.5);
    float sr = sin(eulerAngles.x * 0.5);

    return Quaternion(
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    );
}