#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// survey_len�� �迭 survey�� �����Դϴ�.
// choices_len�� �迭 choices�� �����Դϴ�.
// �Ķ���ͷ� �־����� ���ڿ��� const�� �־����ϴ�. �����Ϸ��� ���ڿ��� �����ؼ� ����ϼ���.
char* solution(const char* survey[], size_t survey_len, int choices[], size_t choices_len) {
    // ���� ������ ���� �ʱ�ȭ
    int scores[8] = { 0 }; // R, T, C, F, J, M, A, N
    char types[] = "RTCFJMAN";

    // �� ������ ���� ���� ó��
    for (size_t i = 0; i < survey_len; i++) {
        char type1 = survey[i][0];
        char type2 = survey[i][1];
        int choice = choices[i];

        int score = 0;
        if (choice < 4) {//��������
            score = 4 - choice;
            for (int j = 0; j < 8; j++) {
                if (types[j] == type1) {
                    scores[j] += score;
                    break;
                }
            }
        }
        else if (choice > 4) {//��������
            score = choice - 4;
            for (int j = 0; j < 8; j++) {
                if (types[j] == type2) {
                    scores[j] += score;
                    break;
                }
            }
        }
    }

    // ���� ���� ����
    char* answer = (char*)malloc(5 * sizeof(char));
    answer[4] = '\0';

    answer[0] = (scores[0] >= scores[1]) ? 'R' : 'T'; // R or T
    answer[1] = (scores[2] >= scores[3]) ? 'C' : 'F'; // C or F
    answer[2] = (scores[4] >= scores[5]) ? 'J' : 'M'; // J or M
    answer[3] = (scores[6] >= scores[7]) ? 'A' : 'N'; // A or N

    return answer;
}

int main() {

    // 1�� �����
    const char* survey1[] = { "AN", "CF", "MJ", "RT", "NA" };
    int choices1[] = { 5, 3, 2, 7, 5 };
    char* result1 = solution(survey1, 5, choices1, 5);
    printf("%s\n", result1);
    free(result1);

    // 2�� �����
    const char* survey2[] = { "TR", "RT", "TR" };
    int choices2[] = { 7, 1, 3 };
    char* result2 = solution(survey2, 3, choices2, 3);
    printf("%s\n", result2);
    free(result2);

    return 0;
}