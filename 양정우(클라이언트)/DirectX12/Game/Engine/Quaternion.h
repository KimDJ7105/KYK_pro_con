#ifndef QUATERNION_H
#define QUATERNION_H

struct Quaternion {
    float x, y, z, w;

    Quaternion(float x, float y, float z, float w);

    // 쿼터니언 곱셈 연산
    Quaternion operator*(const Quaternion& q) const;

    // 쿼터니언의 회전 변환
    Vec3 Rotate(const Vec3& v) const;

    // 쿼터니언을 오일러 각도로 변환
    Vec3 ToEulerAngles() const;

    // 정적 함수: 오일러 각도로부터 쿼터니언을 생성
    static Quaternion FromEulerAngles(const Vec3& eulerAngles);
};

Quaternion QuaternionFromAxisAngle(const Vec3& axis, float angle);

#endif // QUATERNION_H