#pragma once
#include "MainEngine/Level.h"
class SinglePlayerSelectionLevel :
    public Level
{
public:
    SinglePlayerSelectionLevel();

    virtual void Update() override;
    virtual void BeginLevel() override;

    void SetSelectionValue(bool bIsLeft, int value);
    std::pair<int, int> GetSelection() const;

private:
    std::pair<int, int> selection = {0, 0};
};

