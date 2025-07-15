#include "NicMeasurePensando.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <regex>
#include <sys/wait.h>

using namespace std;

NicMeasurePensando::NicMeasurePensando(const std::string& name, const std::string& lif)
    : _name(name),
      _lif(lif)
{}

unsigned long NicMeasurePensando::GetCNPCount()
{
  string json_str;
  unsigned long result = 0;

  try
  {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "/usr/sbin/nicctl show rdma queue-pair statistics --queue-pair-id 1 --lif %s --json",
      _lif.c_str());
    json_str = GetJSON(cmd);

    json json_obj;
    ParseJSON(json_str, json_obj);
    auto nic0 = json_obj["nic"][0];
    auto lif = nic0["lif"][0];
    auto queue_pair = lif["queue_pair"][0];
    auto send_queue = queue_pair["send_queue"];
    auto requester_tx_statistics = send_queue["requester_tx_statistics"];
    auto cnp_str = requester_tx_statistics["num_cnp_sent"];
    result = (unsigned long)atol(cnp_str.dump(4).c_str());
  }
  catch (nlohmann::detail::parse_error& er)
  {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "Error parsing JSON: %s", er.what());
    throw string(buffer);
  }

  return result;
}

// Split string to tokens, with blanks as separators.
void split_string(const string& input, vector<string>& vec)
{
  regex del("\\s+");
  sregex_token_iterator it(input.begin(), input.end(), del, -1);
  sregex_token_iterator end;
  while (it != end)
  {
    vec.push_back(*it);
    ++it;
  }
}

// Convert vector of strings to char** array, where
// the last element of the array is nullptr.
// The result is suitable for passing to execv.
char** convert_vec_to_array(const vector<string>& vec)
{
  char** array;
  int len = vec.size();
  assert(len > 0);
  array = (char**)calloc(len + 1, sizeof(char*));
  for (int i = 0; i < vec.size(); ++i)
  {
    char* newstr = strdup(vec[i].c_str());
    array[i] = newstr;
  }
  return array;
}

std::string NicMeasurePensando::GetJSON(const std::string& command)
{
  int result;
  int pid;
  int pipefd[2];

  result = pipe(pipefd);
  if (result < 0)
  {
    throw "Error in pipe";
  }

  pid = fork();
  if (pid < 0)
  {
    perror("fork");
    exit(1);
  }

  if (pid == 0)
  {
    // Child
    vector<string> vec;
    split_string(command, vec);
    char** array = convert_vec_to_array(vec);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    execv(array[0], array);
    // If we get here, that means execv failed.
    _exit(1);
  }

  // Parent
  close(pipefd[1]); // Close the writing end of the pipe.
  FILE *cmd_output = fdopen(pipefd[0], "r");
  if (cmd_output == nullptr)
  {
    perror("fdopen");
    exit(1);
  }
  char buf[1024];
  string output;
  while (true)
  {
    memset(buf, 0, sizeof(buf));
    if (!fgets(buf, sizeof(buf), cmd_output))
      break;
    output.append(buf);
  }

  int status;
  if (waitpid(pid, &status, 0) < 0)
  {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }

  // DEBUG
  // cout << "GetJSON: " << output << endl;

  return output;
}

void NicMeasurePensando::ParseJSON(const std::string& json_str, json &json_obj)
{
    stringstream stream;
    stream << json_str;
    stream >> json_obj;
}

