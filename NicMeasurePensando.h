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
    std::string _card;

public:
    NicMeasurePensando(const std::string& name, const std::string& lif, const std::string& card);

    // Get Congestion Notification Packet count.
    virtual unsigned long GetCNPCount();

    // Get traffic metrics.
    // transmit packets per second
    //   - tx_pps - transmit packets per second
    //   - tx_bps - transmit bits per second
    //   - rx_pps - receive packets per second
    //   - rx_bps - receive bits per second
    void GetRxTx(
      unsigned long& tx_pps,
      unsigned long& tx_bps,
      unsigned long& rx_pps,
      unsigned long& rx_bps
    );

private:
    static std::string GetJSON(const std::string& command);
    static void ParseJSON(const std::string& json_str, json &json_obj);

};

