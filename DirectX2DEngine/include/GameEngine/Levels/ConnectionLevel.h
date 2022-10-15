#pragma once

#include "MainEngine/Level.h"

class ConnectionLevel :
    public Level
{
public:
    ConnectionLevel();

    virtual void    Update() override;
    virtual void    BeginLevel() override;
};

