//-----------------------------------------------
// 각 칸마다 색이 칠해진 2차원 격자 보드판이 있습니다. 
// 그중 한 칸을 골랐을 때, 위, 아래, 왼쪽, 오른쪽 칸 중 같은 색깔로 칠해진 칸의 개수를 구하려고 합니다.
//보드의 각 칸에 칠해진 색깔 이름이 담긴 이차원 문자열 리스트 board와 고른 칸의 위치를 나타내는 두 정수 h, w가 주어질 때 
// board[h][w]와 이웃한 칸들 중 같은 색으로 칠해져 있는 칸의 개수를 return 하도록 solution 함수를 완성해 주세요.
//이웃한 칸들 중 몇 개의 칸이 같은 색으로 색칠되어 있는지 확인하는 과정은 다음과 같습니다.
// 
// 테스트 데이터 : (보드 정보, h , w, 결과)
// [["blue", "red", "orange", "red"], ["red", "red", "blue", "orange"], ["blue", "orange", "red", "red"], ["orange", "orange", "red", "blue"]]	1	1	2
// [["yellow", "green", "blue"], ["blue", "green", "yellow"], ["yellow", "blue", "blue"]]	                                                    0	1	1
//-----------------------------------------------

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(vector<vector<string>> board, int h, int w) {
    int answer = 0;
    int h_max = board.size() - 1;
    int w_max = board[h].size() - 1;

    vector<int> dh{ 0, 1, -1 ,0 };
    vector<int> dw{ 1, 0, 0 ,-1 };

    for (int i = 0; i < 4; ++i) {
        int temp_h = h + dh[i];
        int temp_w = w + dw[i];

        if (temp_h < 0 || temp_h > h_max) continue;
        if (temp_w < 0 || temp_w > w_max) continue;

        if (board[temp_h][temp_w] == board[h][w]) ++answer;
    }

    return answer;
}

int main() {
    vector<vector<string>> board{ {"blue", "red", "orange", "red"} ,{"red", "red", "blue", "orange"},{"blue", "orange", "red", "red"},{"orange", "orange", "red", "blue"} };
    int h = 0;
    int w = 1;
    std::cout << solution(board,h,w) << std::endl;
}