#include <getopt.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

void display_help(const char* prog) {
  cout << prog << endl
       << "Usage: " << prog << endl
       << "          -l,--lif   - lif (logical interface)" << endl
       << "          -c,--card  - card ID" << endl
       << "          -h,--help  - help message" << endl
       ;
  exit(0);
}


int main(int argc, char* argv[])
{
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

  cout << "lif: " << opt_lif << endl;
  cout << "card: " << opt_card << endl;

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "./nicctl-runner --lif %s --card %s",
    opt_lif.c_str(), opt_card.c_str());

  FILE *stream = popen(cmd, "r");
  if (stream == NULL)
  {
    perror("popen");
    exit(1);
  }
  char buffer[1024];
  string capture;
  while (fgets(buffer, sizeof(buffer), stream) != NULL)
  {
    capture.append(buffer);
  }
  json json_obj;
  stringstream capture_stream;
  capture_stream << capture;
  capture_stream >> json_obj;
  cout << "NIC = " << json_obj["nic"] << endl;
  cout << "CNP = " << json_obj["CNP"] << endl;

  pclose(stream);
  return 0;
}
