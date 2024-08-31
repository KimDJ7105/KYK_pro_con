//-----------------------------------------------
// AI 엔지니어인 현식이는 데이터를 분석하는 작업을 진행하고 있습니다. 
// 데이터는 ["코드 번호(code)", "제조일(date)", "최대 수량(maximum)", "현재 수량(remain)"]으로 구성되어 있으며 
// 현식이는 이 데이터들 중 조건을 만족하는 데이터만 뽑아서 정렬하려 합니다.
// 
// 정렬한 데이터들이 담긴 이차원 정수 리스트 data와 
// 어떤 정보를 기준으로 데이터를 뽑아낼지를 의미하는 문자열 ext, 
// 뽑아낼 정보의 기준값을 나타내는 정수 val_ext, 
// 정보를 정렬할 기준이 되는 문자열 sort_by가 주어집니다.
// data에서 ext 값이 val_ext보다 작은 데이터만 뽑은 후, 
// sort_by에 해당하는 값을 기준으로 오름차순으로 정렬하여 return 하도록 
// solution 함수를 완성해 주세요.
// 단, 조건을 만족하는 데이터는 항상 한 개 이상 존재합니다.
// 
// 테스트 데이터 : (data, 기준값 종류, 기준값, 정렬기준대상, 결과)
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