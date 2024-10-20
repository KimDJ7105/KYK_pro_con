//-----------------------------------------------
// 프로그래머스 "추억점수"
// 
// 사진들을 보며 추억에 젖어 있던 루는 사진별로 추억 점수를 매길려고 합니다. 
// 사진 속에 나오는 인물의 그리움 점수를 모두 합산한 값이 해당 사진의 추억 점수가 됩니다. 
// 예를 들어 사진 속 인물의 이름이 ["may", "kein", "kain"]이고 
// 각 인물의 그리움 점수가 [5점, 10점, 1점]일 때 해당 사진의 추억 점수는 16(5 + 10 + 1)점이 됩니다. 
// 다른 사진 속 인물의 이름이 ["kali", "mari", "don", "tony"]이고 
// ["kali", "mari", "don"]의 그리움 점수가 각각 [11점, 1점, 55점]]이고, 
// "tony"는 그리움 점수가 없을 때, 이 사진의 추억 점수는 3명의 그리움 점수를 합한 67(11 + 1 + 55)점입니다.
// 
// 그리워하는 사람의 이름을 담은 문자열 배열 name, 각 사람별 그리움 점수를 담은 정수 배열 yearning, 
// 각 사진에 찍힌 인물의 이름을 담은 이차원 문자열 배열 photo가 매개변수로 주어질 때, 
// 사진들의 추억 점수를 photo에 주어진 순서대로 배열에 담아 return하는 solution 함수를 완성해주세요.
// 
// 테스트 데이터 : (순서대로 name, yearning, photo, 정답)
// ["may", "kein", "kain", "radi"]	[5, 10, 1, 3]	[["may", "kein", "kain", "radi"],["may", "kein", "brin", "deny"], ["kon", "kain", "may", "coni"]]	[19, 15, 6]
// ["kali", "mari", "don"]	[11, 1, 55]	[["kali", "mari", "don"], ["pony", "tom", "teddy"], ["con", "mona", "don"]]	[67, 0, 55]
// ["may", "kein", "kain", "radi"]	[5, 10, 1, 3]	[["may"],["kein", "deny", "may"], ["kon", "coni"]]	[5, 15, 0]
//-----------------------------------------------


#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<string> name, vector<int> yearning, vector<vector<string>> photo) {
    vector<int> answer;

    for (auto& peoples : photo) {
        int point{ 0 };
        for (auto& names : peoples) {  //사진속 인물 중 추억이 있는 사람이 있는지 확인

            auto it = find(name.begin(), name.end(), names);
            if (it == name.end()) continue; //해당 인물이 없으면 통과

            int index = std::distance(name.begin(), it);

            point += yearning[index];
        }

        answer.emplace_back(point);
    }

    return answer;
}

int main() {
    vector<string> name{ "kali", "mari", "don" };
    vector<int> yearning{ 11, 1, 55 };
    vector<vector<string>> photo{ {"kali", "mari", "don" }, { "pony", "tom", "teddy" }, { "con", "mona", "don" }};

    auto result = solution(name, yearning, photo);

    for (auto& a : result) std::cout << a << std::endl;
}