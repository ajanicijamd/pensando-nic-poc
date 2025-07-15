#include <iostream>
#include "NicMeasurePensando.h"

using namespace std;

void usage(const char* prog)
{
  fprintf(stderr, "%s\nUsage: %s <lif-id>\n", prog, prog);
  exit(0);
}

int main(int argc, char* argv[]) {
  if (argc < 2)
  {
    usage(argv[0]);
  }

  auto lif = argv[1];
  char cmd[256];

  string name("myniccard");
  NicMeasurePensando nic(name, lif);

  unsigned long cnp = 0;

  try
  {
    cnp = nic.GetCNPCount();
  }
  catch (const string& err)
  {
    cout << err << endl;
    exit(1);
  }
  json json_out;
  json_out["nic"] = name.c_str();
  json_out["CNP"] = cnp;
  cout << json_out.dump(2) << endl;
  return 0;
}
