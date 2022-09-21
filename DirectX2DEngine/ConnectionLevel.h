#pragma once
#include "Level.h"
class ConnectionLevel :
    public Level
{
public:
    ConnectionLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;
};

