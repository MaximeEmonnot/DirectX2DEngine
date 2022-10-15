#pragma once
#include <fstream>

#include "Logger.h"
class ConsoleLogger :
    public Logger
{
public:
    ConsoleLogger();
    virtual ~ConsoleLogger() override;

protected:
    void            LogMessage(const std::string& message) override;

private:
    std::ofstream   out;
};

