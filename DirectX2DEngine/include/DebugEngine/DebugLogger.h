#pragma once
#include <fstream>

#include "Logger.h"
class DebugLogger :
    public Logger
{
public:
    DebugLogger();
    virtual ~DebugLogger() override;

protected:
    void            LogMessage(const std::string& message) override;

private:
    std::ofstream   out;
};

