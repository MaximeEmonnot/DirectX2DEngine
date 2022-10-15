#pragma once
#include <fstream>

#include "Logger.h"
class ErrorLogger :
    public Logger
{
public:
    ErrorLogger();
    virtual ~ErrorLogger() override;

protected:
    void LogMessage(const std::string& message) override;

private:
    std::ofstream out;
};
