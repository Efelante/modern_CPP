#include <string>
#include <vector>
using namespace std;

#define STRINGIFY(x) id##x
#define STR(x) STRINGIFY(x) 
#define UNIQ_ID STR(__LINE__) 

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}
