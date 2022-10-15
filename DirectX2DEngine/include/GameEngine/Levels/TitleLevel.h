#pragma once
#include "MainEngine/Level.h"
class TitleLevel :
    public Level
{
public:
    TitleLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;
};

