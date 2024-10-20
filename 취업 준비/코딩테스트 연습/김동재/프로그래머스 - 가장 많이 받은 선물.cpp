//-----------------------------------------------
// 선물을 직접 전하기 힘들 때 카카오톡 선물하기 기능을 이용해 축하 선물을 보낼 수 있습니다. 
// 당신의 친구들이 이번 달까지 선물을 주고받은 기록을 바탕으로 다음 달에 누가 선물을 많이 받을지 예측하려고 합니다.
// 
// 두 사람이 선물을 주고받은 기록이 있다면, 이번 달까지 두 사람 사이에 더 많은 선물을 준 사람이 다음 달에 선물을 하나 받습니다.
// 예를 들어 A가 B에게 선물을 5번 줬고, B가 A에게 선물을 3번 줬다면 다음 달엔 A가 B에게 선물을 하나 받습니다.
// 
// 두 사람이 선물을 주고받은 기록이 하나도 없거나 주고받은 수가 같다면, 선물 지수가 더 큰 사람이 선물 지수가 더 작은 사람에게 선물을 하나 받습니다.
// 
// 선물 지수는 이번 달까지 자신이 친구들에게 준 선물의 수에서 받은 선물의 수를 뺀 값입니다.
// 예를 들어 A가 친구들에게 준 선물이 3개고 받은 선물이 10개라면 A의 선물 지수는 - 7입니다.
// B가 친구들에게 준 선물이 3개고 받은 선물이 2개라면 B의 선물 지수는 1입니다.
// 만약 A와 B가 선물을 주고받은 적이 없거나 정확히 같은 수로 선물을 주고받았다면, 다음 달엔 B가 A에게 선물을 하나 받습니다.
// 만약 두 사람의 선물 지수도 같다면 다음 달에 선물을 주고받지 않습니다.
// 
// 위에서 설명한 규칙대로 다음 달에 선물을 주고받을 때, 당신은 선물을 가장 많이 받을 친구가 받을 선물의 수를 알고 싶습니다.
// 친구들의 이름을 담은 1차원 문자열 배열 friends 이번 달까지 친구들이 주고받은 선물 기록을 담은 1차원 문자열 배열 gifts가 매개변수로 주어집니다.
// 이때, 다음달에 가장 많은 선물을 받는 친구가 받을 선물의 수를 return 하도록 solution 함수를 완성해 주세요.
// 
// 테스트 데이터 : (친구들 이름, 선물 주고 받은 이력, 결과)
// ["muzi", "ryan", "frodo", "neo"]	                ["muzi frodo", "muzi frodo", "ryan muzi", "ryan muzi", "ryan muzi", "frodo muzi", "frodo ryan", "neo muzi"]	    2
// ["joy", "brad", "alessandro", "conan", "david"]	["alessandro brad", "alessandro joy", "alessandro conan", "david alessandro", "alessandro david"]	            4
// ["a", "b", "c"]	                                ["a b", "b a", "c a", "a c", "a c", "c a"]	                                                                    0
//-----------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

int solution(vector<string> friends, vector<string> gifts) {
    map<string, int> gift_point;
    map<string, int> expect_gift;

    for (const auto& f : friends) {
        gift_point[f] = 0;
        expect_gift[f] = 0;
    }

    //선물 지수 계산

    for (auto& g : gifts) {
        std::string sender, recver;
        std::stringstream ss(g);

        ss >> sender >> recver;

        gift_point[sender] += 1;
        gift_point[recver] -= 1;
    }

    //어느 친구가 가장 선물을 많이 받을지 계산
    for (int i = 0; i < friends.size(); ++i) {
        for (int j = i + 1; j < friends.size(); ++j) {
            int i_send = std::count(gifts.begin(), gifts.end(), friends[i] + " " + friends[j]);
            int j_send = std::count(gifts.begin(), gifts.end(), friends[j] + " " + friends[i]);

            if (i_send > j_send) expect_gift[friends[i]] += 1;
            else if (i_send < j_send) expect_gift[friends[j]] += 1;
            else if (i_send == 0 && j_send == 0 || i_send == j_send) {
                if (gift_point[friends[i]] > gift_point[friends[j]]) expect_gift[friends[i]] += 1;
                else if (gift_point[friends[i]] < gift_point[friends[j]])expect_gift[friends[j]] += 1;
            }
        }
    }

    auto most_gifted = std::max_element(
        expect_gift.begin(), expect_gift.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second < b.second;
        }
    );

    return most_gifted->second;
}

int main() {
    vector<string> friends{ "muzi", "ryan", "frodo", "neo" };
    vector<string> gifts{ "muzi frodo", "muzi frodo", "ryan muzi", "ryan muzi", "ryan muzi", "frodo muzi", "frodo ryan", "neo muzi" };

    std::cout << solution(friends, gifts);
}