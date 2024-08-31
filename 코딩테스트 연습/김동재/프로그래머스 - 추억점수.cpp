//-----------------------------------------------
// ���α׷��ӽ� "�߾�����"
// 
// �������� ���� �߾￡ ���� �ִ� ��� �������� �߾� ������ �ű���� �մϴ�. 
// ���� �ӿ� ������ �ι��� �׸��� ������ ��� �ջ��� ���� �ش� ������ �߾� ������ �˴ϴ�. 
// ���� ��� ���� �� �ι��� �̸��� ["may", "kein", "kain"]�̰� 
// �� �ι��� �׸��� ������ [5��, 10��, 1��]�� �� �ش� ������ �߾� ������ 16(5 + 10 + 1)���� �˴ϴ�. 
// �ٸ� ���� �� �ι��� �̸��� ["kali", "mari", "don", "tony"]�̰� 
// ["kali", "mari", "don"]�� �׸��� ������ ���� [11��, 1��, 55��]]�̰�, 
// "tony"�� �׸��� ������ ���� ��, �� ������ �߾� ������ 3���� �׸��� ������ ���� 67(11 + 1 + 55)���Դϴ�.
// 
// �׸����ϴ� ����� �̸��� ���� ���ڿ� �迭 name, �� ����� �׸��� ������ ���� ���� �迭 yearning, 
// �� ������ ���� �ι��� �̸��� ���� ������ ���ڿ� �迭 photo�� �Ű������� �־��� ��, 
// �������� �߾� ������ photo�� �־��� ������� �迭�� ��� return�ϴ� solution �Լ��� �ϼ����ּ���.
// 
// �׽�Ʈ ������ : (������� name, yearning, photo, ����)
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
        for (auto& names : peoples) {  //������ �ι� �� �߾��� �ִ� ����� �ִ��� Ȯ��

            auto it = find(name.begin(), name.end(), names);
            if (it == name.end()) continue; //�ش� �ι��� ������ ���

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