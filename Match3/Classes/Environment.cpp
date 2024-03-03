#include "stdafx.h"
#include "Environment.h"

Global* instance = nullptr;

Global* Global::getInstance()
{
    if (instance == nullptr) instance = new Global();
    return instance;
}

void Global::setScore(long long value)
{
    this->score = value;
}

long long Global::getScore()
{

    return score;
}

void Global::addScore(long long value)
{
    this->score += value;
}
