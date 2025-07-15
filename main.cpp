#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

void usage(const char* prog)
{
  fprintf(stderr, "%s\nUsage: %s <lif-id>\n", prog, prog);
  exit(0);
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    usage(argv[0]);
  }

  auto lif = argv[1];
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "./nicctl-runner %s", lif);

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
