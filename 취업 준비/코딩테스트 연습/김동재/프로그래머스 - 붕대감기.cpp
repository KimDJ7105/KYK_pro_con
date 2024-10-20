//-----------------------------------------------
// 어떤 게임에는 붕대 감기라는 기술이 있습니다.
// 붕대 감기는 t초 동안 붕대를 감으면서 1초마다 x만큼의 체력을 회복합니다.
// t초 연속으로 붕대를 감는 데 성공한다면 y만큼의 체력을 추가로 회복합니다.
// 게임 캐릭터에는 최대 체력이 존재해 현재 체력이 최대 체력보다 커지는 것은 불가능합니다.
// 기술을 쓰는 도중 몬스터에게 공격을 당하면 기술이 취소되고, 공격을 당하는 순간에는 체력을 회복할 수 없습니다.
// 몬스터에게 공격당해 기술이 취소당하거나 기술이 끝나면 그 즉시 붕대 감기를 다시 사용하며, 연속 성공 시간이 0으로 초기화됩니다.
// 몬스터의 공격을 받으면 정해진 피해량만큼 현재 체력이 줄어듭니다.
// 이때, 현재 체력이 0 이하가 되면 캐릭터가 죽으며 더 이상 체력을 회복할 수 없습니다.
//
// 당신은 붕대감기 기술의 정보, 캐릭터가 가진 최대 체력과 몬스터의 공격 패턴이 주어질 때 캐릭터가 끝까지 생존할 수 있는지 궁금합니다.
// 
// 붕대 감기 기술의 시전 시간, 1초당 회복량, 추가 회복량을 담은 1차원 정수 배열 bandage와 
// 최대 체력을 의미하는 정수 health, 몬스터의 공격 시간과 피해량을 담은 2차원 정수 배열 attacks가 매개변수로 주어집니다.
// 모든 공격이 끝난 직후 남은 체력을 return 하도록 solution 함수를 완성해 주세요.만약 몬스터의 공격을 받고 
// 캐릭터의 체력이 0 이하가 되어 죽는다면 - 1을 return 해주세요.
// 
// 테스트 데이터 (bandage, health, attacks, 결괏값)
// [5, 1, 5]	    30	    [[2, 10], [9, 15], [10, 5], [11, 5]]	    5
// [3, 2, 7]	    20	    [[1, 15], [5, 16], [8, 6]]	                -1
// [3, 2, 7]	    20	    [[1, 15], [5, 16], [8, 6]]	                -1
// [1, 1, 1]	    5	    [[1, 2], [3, 2]]	                        3
//-----------------------------------------------

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(vector<int> bandage, int health, vector<vector<int>> attacks) {
    int hp = health;
    int using_count{ 0 };
    int elapsed_time{ 1 };


    while (hp > 0 && !attacks.empty()) {
        if (attacks[0][0] == elapsed_time) {
            //몬스터 공격 시간이면 피해량 만큼 체력을 깎고, 붕대 사용 시간을 초기화 한다.
            hp -= attacks[0][1];
            attacks.erase(attacks.begin());
            using_count = 0;
            ++elapsed_time;
            continue;
        }
        //몬스터에게 공격받지 않으면 회복량 만큼 회복하고 
        //사용 시간이 다 됐으면 추가 회복을 진행한다.
        //최대체력보다 체력량이 많으면 최대체력으로 수정한다.

        hp += bandage[1];
        if (++using_count == bandage[0]) { //사용 시간을 증가시키고 시전 시간과 비교한다.
            using_count = 0; //만일 시전이 끝났으면 사용 시간을 0으로 변경하고 추가 회복 실시
            hp += bandage[2];
        }

        if (hp > health) hp = health; //최대 체력 이상 회복 불가.

        //시간을 진행시킨다.
        ++elapsed_time;
    }

    if (hp <= 0) return -1;

    return hp;
}

int main() {
    vector<int> bandage{ 1,1,1 };
    int hp{ 5 };
    vector<vector<int>> attacks{ {1, 2} ,{3, 2} };

    cout << solution(bandage, hp, attacks);
}