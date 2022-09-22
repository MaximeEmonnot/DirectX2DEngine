#pragma once
#include "Level.h"
class MultiPlayerSelectionLevel :
    public Level
{
public:
    MultiPlayerSelectionLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;

    void SetSelectionValue(int value);
    int GetSelection() const;

private:
    int selection = 0;
};

