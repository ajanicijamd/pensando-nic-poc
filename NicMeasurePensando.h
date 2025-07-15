#pragma once

#include "NicMeasure.h"
#include <string>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class NicMeasurePensando : public NicMeasure
{
private:
    std::string _name;
    std::string _lif;

public:
    NicMeasurePensando(const std::string& name, const std::string& lif);

    virtual unsigned long GetCNPCount();
    void ParseJSON(const std::string& json_str, json &json_obj);

private:
    std::string GetJSON(const std::string& command);

};

