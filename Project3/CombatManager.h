#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_map>
#include "Monster.h"
#include "MonsterManager.h"
#include "GameManager.h"
#include "Renderer.h"
#include "BattleManager.h"
#include "Actor.h"


class CombatManager
{
private:
    std::vector<std::string> areaName = {
    "Forest", "Desert", "Cave"
    };

    //Renderer Class에 대한 참조
    Renderer* renderer;
    MonsterManager* monsterManager;
    BattleManager* battleManager;

    std::unordered_map<std::string, std::string> areaDisplayname =
    {
        {"Forest", " 숲 "},
        {"Desert", " 사막"},
        {"Cave", " 동굴"},
        {"Temple", " 신전"},
        {"Jungle", " 정글"},
        {"Mine", " 폐광"},
        {"Ocean", " 해저"},
        {"Alley", "뒷골목"},
        {"Mountain", " 산 "},
        {"Nest", " 둥지"}
    };

    //플레이어에게 보여줄 선택지 갯수
    std::vector<std::string> currentChoices;

public:
    //Renderer Class을 포함한 생성자
    CombatManager(Renderer* r, MonsterManager* m) : renderer(r), monsterManager(m) {
        battleManager = new BattleManager(r);
    };

    ~CombatManager() {delete battleManager;}

    //맵 무작위 생성 (3개)
    void GenerateAreaChoices();
    //맵 생성된거 3개 보여주고 어떤거 고를지 말하는 옵션
    void DisplayChoices();
    //입력받아서 MonsterManager에 전달해서 해당 맵에 몬스터 스폰
    std::string SelectArea();


    //맵을 무작위로 생성하고, 선택지 보여주는 함수
    void Run(Character* player);

    //체력깍는 로직
    void ReduceHp(Actor* actor, Actor* monster);

};


