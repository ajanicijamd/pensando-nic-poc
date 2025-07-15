#pragma once

class NicMeasure
{
public:
    // Get Congestion Notification Packet count.
    virtual unsigned long GetCNPCount() = 0;

    // Get traffic metrics.
    // transmit packets per second
    //   - tx_pps - transmit packets per second
    //   - tx_bps - transmit bits per second
    //   - rx_pps - receive packets per second
    //   - rx_bps - receive bits per second
    virtual void GetRxTx(
      unsigned long& tx_pps,
      unsigned long& tx_bps,
      unsigned long& rx_pps,
      unsigned long& rx_bps
    ) = 0;
};
