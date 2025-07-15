#include <iostream>
#include "NicMeasurePensando.h"
#include <getopt.h>

using namespace std;

void display_help(const char* prog) {
  cout << prog << endl
       << "Usage: " << prog << endl
       << "          -l,--lif   - lif (logical interface)" << endl
       << "          -c,--card  - card ID" << endl
       << "          -h,--help  - help message" << endl
       ;
  exit(0);
}


int main(int argc, char* argv[]) {
  if (argc < 2)
  {
    display_help(argv[0]);
  }

  // Process command line arguments.
  int c;
  string opt_lif;
  string opt_card;
  bool fl_help(false);

  while (true) {
    static struct option long_options[] = {
      {"lif", required_argument, 0, 'l'},
      {"card", required_argument, 0, 'c'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0}
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "l:c:h",
      long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
      case 'h':
        fl_help = true;
        break;
      case 'l':
        // cout << "Option -l with value " << optarg << endl;
        opt_lif = optarg;
        break;
      case 'c':
        // cout << "Option -c with value " << optarg << endl;
        opt_card = optarg;
        break;
      default:
        break;
    }
  }

  if (fl_help)
  {
     display_help(argv[0]);
  }

  char cmd[256];

  string name("myniccard");
  NicMeasurePensando nic(name, opt_lif, opt_card);

  unsigned long cnp = 0;
  unsigned long tx_pps, tx_bps, rx_pps, rx_bps;

  try
  {
    cnp = nic.GetCNPCount();
    nic.GetRxTx(tx_pps, tx_bps, rx_pps, rx_bps);
  }
  catch (const string& err)
  {
    cout << err << endl;
    exit(1);
  }
  json json_out;
  json_out["nic"] = name.c_str();
  json_out["CNP"] = cnp;
  json_out["tx_pps"] = tx_pps;
  json_out["tx_bps"] = tx_bps;
  json_out["rx_pps"] = rx_pps;
  json_out["rx_bps"] = rx_bps;
  cout << json_out.dump() << endl;
  return 0;
}
