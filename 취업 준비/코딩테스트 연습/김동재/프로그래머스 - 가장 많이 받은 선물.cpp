//-----------------------------------------------
// ������ ���� ���ϱ� ���� �� īī���� �����ϱ� ����� �̿��� ���� ������ ���� �� �ֽ��ϴ�. 
// ����� ģ������ �̹� �ޱ��� ������ �ְ���� ����� �������� ���� �޿� ���� ������ ���� ������ �����Ϸ��� �մϴ�.
// 
// �� ����� ������ �ְ���� ����� �ִٸ�, �̹� �ޱ��� �� ��� ���̿� �� ���� ������ �� ����� ���� �޿� ������ �ϳ� �޽��ϴ�.
// ���� ��� A�� B���� ������ 5�� ���, B�� A���� ������ 3�� ��ٸ� ���� �޿� A�� B���� ������ �ϳ� �޽��ϴ�.
// 
// �� ����� ������ �ְ���� ����� �ϳ��� ���ų� �ְ���� ���� ���ٸ�, ���� ������ �� ū ����� ���� ������ �� ���� ������� ������ �ϳ� �޽��ϴ�.
// 
// ���� ������ �̹� �ޱ��� �ڽ��� ģ���鿡�� �� ������ ������ ���� ������ ���� �� ���Դϴ�.
// ���� ��� A�� ģ���鿡�� �� ������ 3���� ���� ������ 10����� A�� ���� ������ - 7�Դϴ�.
// B�� ģ���鿡�� �� ������ 3���� ���� ������ 2����� B�� ���� ������ 1�Դϴ�.
// ���� A�� B�� ������ �ְ���� ���� ���ų� ��Ȯ�� ���� ���� ������ �ְ�޾Ҵٸ�, ���� �޿� B�� A���� ������ �ϳ� �޽��ϴ�.
// ���� �� ����� ���� ������ ���ٸ� ���� �޿� ������ �ְ���� �ʽ��ϴ�.
// 
// ������ ������ ��Ģ��� ���� �޿� ������ �ְ���� ��, ����� ������ ���� ���� ���� ģ���� ���� ������ ���� �˰� �ͽ��ϴ�.
// ģ������ �̸��� ���� 1���� ���ڿ� �迭 friends �̹� �ޱ��� ģ������ �ְ���� ���� ����� ���� 1���� ���ڿ� �迭 gifts�� �Ű������� �־����ϴ�.
// �̶�, �����޿� ���� ���� ������ �޴� ģ���� ���� ������ ���� return �ϵ��� solution �Լ��� �ϼ��� �ּ���.
// 
// �׽�Ʈ ������ : (ģ���� �̸�, ���� �ְ� ���� �̷�, ���)
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

    //���� ���� ���

    for (auto& g : gifts) {
        std::string sender, recver;
        std::stringstream ss(g);

        ss >> sender >> recver;

        gift_point[sender] += 1;
        gift_point[recver] -= 1;
    }

    //��� ģ���� ���� ������ ���� ������ ���
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