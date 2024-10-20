//-----------------------------------------------
// AI �����Ͼ��� �����̴� �����͸� �м��ϴ� �۾��� �����ϰ� �ֽ��ϴ�. 
// �����ʹ� ["�ڵ� ��ȣ(code)", "������(date)", "�ִ� ����(maximum)", "���� ����(remain)"]���� �����Ǿ� ������ 
// �����̴� �� �����͵� �� ������ �����ϴ� �����͸� �̾Ƽ� �����Ϸ� �մϴ�.
// 
// ������ �����͵��� ��� ������ ���� ����Ʈ data�� 
// � ������ �������� �����͸� �̾Ƴ����� �ǹ��ϴ� ���ڿ� ext, 
// �̾Ƴ� ������ ���ذ��� ��Ÿ���� ���� val_ext, 
// ������ ������ ������ �Ǵ� ���ڿ� sort_by�� �־����ϴ�.
// data���� ext ���� val_ext���� ���� �����͸� ���� ��, 
// sort_by�� �ش��ϴ� ���� �������� ������������ �����Ͽ� return �ϵ��� 
// solution �Լ��� �ϼ��� �ּ���.
// ��, ������ �����ϴ� �����ʹ� �׻� �� �� �̻� �����մϴ�.
// 
// �׽�Ʈ ������ : (data, ���ذ� ����, ���ذ�, ���ı��ش��, ���)
// [[1, 20300104, 100, 80], [2, 20300804, 847, 37], [3, 20300401, 10, 8]]	"date"	20300501	"remain"	[[3,20300401,10,8],[1,20300104,100,80]]                                                    0	1	1
//-----------------------------------------------

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

vector<vector<int>> solution(vector<vector<int>> data, string ext, int val_ext, string sort_by) {
    vector<vector<int>> answer;
    map<string, int> ext_index{ {"code", 0}, {"data", 1}, {"maximum", 2}, {"remain", 3} };

    std::copy_if(data.begin(), data.end(), std::back_inserter(answer), [&](vector<int> temp) {
        return temp[ext_index[ext]] < val_ext;
        });

    std::sort(answer.begin(), answer.end(), [&](const auto& a, const auto& b) {
        return a[ext_index[sort_by]] < b[ext_index[sort_by]];
        });

    return answer;
}

int main() {
    vector<vector<int>> data{ {1, 20300104, 100, 80} ,{2, 20300804, 847, 37},{3, 20300401, 10, 8} };
    string ext{ "data" };
    int val_ext{ 20300501 };
    string sort_by{ "remain" };

    for (auto& v : solution(data, ext, val_ext, sort_by)) {
        for (auto& d : v) {
            std::cout << d << ", ";
        }
        std::cout << "\n";
    }
}