#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <windows.h>
#include <conio.h>

const int WIDTH = 50;
const int HEIGHT = 25;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

enum ItemType { SPEED_UP, SPEED_DOWN, SIZE_UP, SIZE_DOWN, NONE };

// 기록 저장 구조체
struct GameRecord {
    std::string date;
    double survivalTime;
    int destroyedCount;
    int totalScore;

    // 점수 내림차순 정렬을 위한 비교 연산자
    bool operator>(const GameRecord& other) const {
        return totalScore > other.totalScore;
    }
};

inline void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}