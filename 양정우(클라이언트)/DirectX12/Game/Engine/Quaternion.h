#ifndef QUATERNION_H
#define QUATERNION_H

struct Quaternion {
    float x, y, z, w;

    Quaternion(float x, float y, float z, float w);

    // ���ʹϾ� ���� ����
    Quaternion operator*(const Quaternion& q) const;

    // ���ʹϾ��� ȸ�� ��ȯ
    Vec3 Rotate(const Vec3& v) const;

    // ���ʹϾ��� ���Ϸ� ������ ��ȯ
    Vec3 ToEulerAngles() const;

    // ���� �Լ�: ���Ϸ� �����κ��� ���ʹϾ��� ����
    static Quaternion FromEulerAngles(const Vec3& eulerAngles);
};

Quaternion QuaternionFromAxisAngle(const Vec3& axis, float angle);

#endif // QUATERNION_H