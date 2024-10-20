//-----------------------------------------------
// �� ĭ���� ���� ĥ���� 2���� ���� �������� �ֽ��ϴ�. 
// ���� �� ĭ�� ����� ��, ��, �Ʒ�, ����, ������ ĭ �� ���� ����� ĥ���� ĭ�� ������ ���Ϸ��� �մϴ�.
//������ �� ĭ�� ĥ���� ���� �̸��� ��� ������ ���ڿ� ����Ʈ board�� �� ĭ�� ��ġ�� ��Ÿ���� �� ���� h, w�� �־��� �� 
// board[h][w]�� �̿��� ĭ�� �� ���� ������ ĥ���� �ִ� ĭ�� ������ return �ϵ��� solution �Լ��� �ϼ��� �ּ���.
//�̿��� ĭ�� �� �� ���� ĭ�� ���� ������ ��ĥ�Ǿ� �ִ��� Ȯ���ϴ� ������ ������ �����ϴ�.
// 
// �׽�Ʈ ������ : (���� ����, h , w, ���)
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