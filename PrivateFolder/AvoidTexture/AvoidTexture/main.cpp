#include "Common.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// 날짜 문자열 생성 함수
string GetCurrentDate() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return string(buf);
}

// 파일에서 기록 읽기 (없으면 자동 생성)
vector<GameRecord> LoadRecords() {
    vector<GameRecord> records;

    // 1. 파일이 있는지 먼저 확인하고, 없으면 생성
    ifstream checkFile("scores.txt");
    if (!checkFile.is_open()) {
        // 읽기 실패 시, 쓰기 모드로 열어서 빈 파일을 생성함
        ofstream createFile("scores.txt");
        createFile.close();
    }
    checkFile.close();

    // 2. 이제 파일을 열어서 데이터 읽기
    ifstream file("scores.txt");
    if (file.is_open()) {
        GameRecord r;
        // 데이터가 있을 때만 읽어들임
        while (file >> r.date >> r.survivalTime >> r.destroyedCount >> r.totalScore) {
            records.push_back(r);
        }
        file.close();
    }
    return records;
}

// 파일에 기록 저장
void SaveRecord(GameRecord newRecord) {
    vector<GameRecord> records = LoadRecords();
    records.push_back(newRecord);
    sort(records.begin(), records.end(), greater<GameRecord>());

    ofstream file("scores.txt");
    if (file.is_open()) {
        for (const auto& r : records) {
            file << r.date << " " << r.survivalTime << " " << r.destroyedCount << " " << r.totalScore << endl;
        }
        file.close();
    }
}

int main() {
    srand((unsigned int)time(NULL));

    // 게임 시작 전 파일 체크 및 초기화
    vector<GameRecord> initialCheck = LoadRecords();

    Player player;
    Item item;
    vector<Enemy> enemies;
    int destroyedCount = 0;
    ULONGLONG startTime = GetTickCount64();
    ULONGLONG lastItemTime = startTime;

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(hOut, &ci);

    // main.cpp의 while 루프 진입 직전
    system("cls");
    cout << "AvoidTexture - 준비되셨나요?" << endl;
    cout << "'P'가 당신입니다. 방향키로 움직이세요." << endl;
    Sleep(2000); // 2초간 위치를 확인할 시간을 줌

    while (true) {
        ULONGLONG now = GetTickCount64();
        double elapsed = (now - startTime) / 1000.0;

        // 1. 논리 업데이트
        player.Update(now);
        if (_kbhit()) player.Move(_getch());
        if (now - lastItemTime >= 20000) { item.Spawn(); lastItemTime = now; }
        if (item.active && (int)player.x == item.x && (int)player.y == item.y) {
            player.activeEffect = item.type;
            player.effectEndTime = now + 10000;
            item.active = false;
        }
        if (rand() % 10 < 2) enemies.push_back(Enemy(player.x, player.y));

        for (int i = 0; i < enemies.size(); ) {
            if (enemies[i].Update()) {
                enemies.erase(enemies.begin() + i);
                destroyedCount++;
            }
            else {
                double dist = sqrt(pow(player.x - enemies[i].x, 2) + pow(player.y - enemies[i].y, 2));

                // 충돌 발생 시!!
                if (dist < player.GetSize() * 0.8) {
                    while (_kbhit()) _getch();
                    system("cls");

                    // 1. 현재 기록 생성 및 저장
                    int finalScore = (int)(elapsed * 10) + destroyedCount * 5;
                    GameRecord currentRec = { GetCurrentDate(), elapsed, destroyedCount, finalScore };
                    SaveRecord(currentRec);

                    // 2. GAME OVER 애니메이션 (기존 로직)
                    SetColor(12);
                    string msg[] = { 
                        "      ##########################################", 
                        "      #                                        #",
                        "      #               GAME  OVER               #", 
                        "      #                                        #",
                        "      ##########################################" 
                    };

                    // 2초(2000ms) 동안 천천히 출력하기 위해 각 줄 사이에 간격 부여
                    for (const string& line : msg) {
                        cout << line << endl;
                        Sleep(400); // 5줄이므로 약 2초간 연출 (400ms * 5 = 2000ms)
                    }

                    cout << "\n\n";

                    // 3. 현재 점수 출력
                    SetColor(7);    // 흰색
                    printf("          ▶  버틴 시간 :  %.3f 초\n", elapsed);
                    printf("          ▶  제거된 장애물 횟수 :  %d 개\n", destroyedCount);
                    printf("          ▶  최종 점수 :  %d 점\n", finalScore);

                    // 4. TOP 5 랭킹 출력
                    cout << "      ========= TOP 5 RANKING =========" << endl;
                    vector<GameRecord> allRecords = LoadRecords();

                    // 깜빡임 연출 (5번 반복)
                    for (int blink = 0; blink < 5; blink++) {
                        SetConsoleCursorPosition(hOut, { 0, 12 }); // 랭킹 시작 위치로 커서 고정
                        for (int i = 0; i < 5 && i < allRecords.size(); i++) {
                            bool isNew = (allRecords[i].totalScore == finalScore && allRecords[i].date == currentRec.date);

                            if (isNew && blink % 2 == 0) SetColor(0); // 깜빡일 때 배경색과 같게 숨김
                            else if (isNew) SetColor(14); // 신기록은 노란색
                            else SetColor(7); // 나머지는 흰색

                            printf("      %d. %s | %5.1fs | %3d개 | %d점\n",
                                i + 1, allRecords[i].date.c_str(), allRecords[i].survivalTime,
                                allRecords[i].destroyedCount, allRecords[i].totalScore);
                        }
                        Sleep(300);
                    }

                    // ★ 중요: 루프가 끝난 후 모든 글자 색상을 흰색으로 강제 복구 ★
                    SetColor(7);

                    // 마지막으로 랭킹을 다시 한 번 깔끔하게 출력 (깜빡임 끝난 후 고정 상태)
                    SetConsoleCursorPosition(hOut, { 0, 12 });
                    for (int i = 0; i < 5 && i < allRecords.size(); i++) {
                        if (allRecords[i].totalScore == finalScore) SetColor(14); // 신기록만 강조
                        else SetColor(7);

                        printf("      %d. %s | %5.1fs | %3d개 | %d점  \n",
                            i + 1, allRecords[i].date.c_str(), allRecords[i].survivalTime,
                            allRecords[i].destroyedCount, allRecords[i].totalScore);
                    }

                    SetColor(8); // 안내 문구는 어두운 회색
                    cout << "\n      점수를 확인한 후 아무 키나 누르면 종료됩니다..." << endl;

                    _getch();
                    return 0;
                }
                i++;
            }
        }

        // 2. 렌더링 최적화
        SetConsoleCursorPosition(hOut, { 0, 0 });
        string buffer = ""; // 화면 전체를 담을 버퍼

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                bool drawn = false;

                // 플레이어 (우선순위 1)
                if ((int)player.x == x && (int)player.y == y) {
                    buffer += "P"; // 색상 코드는 문자열 버퍼에 넣기 까다로우므로 일단 기호로 구분
                    drawn = true;
                }
                // 아이템 (우선순위 2)
                else if (item.active && item.x == x && item.y == y) {
                    buffer += "?";
                    drawn = true;
                }
                // 장애물 (우선순위 3)
                else {
                    for (auto& e : enemies) {
                        if ((int)e.x == x && (int)e.y == y) {
                            buffer += e.shape;
                            drawn = true;
                            break;
                        }
                    }
                }

                if (!drawn) buffer += "."; // 배경
            }
            buffer += "|\n";
        }

        cout << buffer; // 단 한 번의 호출로 화면 출력
        printf("Time: %.3f s | Score: %d\n", elapsed, (int)(elapsed * 10) + destroyedCount * 5);
        Sleep(30);
    }
}

