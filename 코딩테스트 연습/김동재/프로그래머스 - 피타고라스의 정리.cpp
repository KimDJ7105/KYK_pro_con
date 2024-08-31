//-----------------------------------------------
//직각삼각형의 한 변의 길이를 나타내는 정수 a와 빗변의 길이를 나타내는 정수 c가 주어질 때, 
// 다른 한 변의 길이의 제곱, b_square 을 출력하도록 한 줄을 수정해 코드를 완성해 주세요.
// 
// int main(void) {
//  int a;
//  int c;
//  cin >> a >> c;
//  
//  int b_square = c - a;
//  cout << b_square << endl;
//  return 0;
//}
//-----------------------------------------------

#include <iostream>

using namespace std;

int main(void) {
    int a;
    int c;
    cin >> a >> c;

    //int b_square = c - a;
    int b_square = c * c - a * a;
    cout << b_square << endl;
    return 0;
}