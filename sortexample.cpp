// sort algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector

bool myfunction (int i,int j) { return (i<j); }

struct myclass {
  bool operator() (int i,int j) { return (i<j);}
} myobject;

int main () {
  int myints[] = {32,71,12,45,26,80,53,33};
  std::vector<int> myvector (myints, myints+8);               // 32 71 12 45 26 80 53 33

  // using default comparison (operator <):
  std::sort (myints, myints+4);           //(12 32 45 71)26 80 53 33

  // print out content:
  std::cout << "myvector contains:";
  for (int * it= &myints[0]; it!= &myints[8]; ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  // using function as comp
  std::sort (myints+4, myints+8, myfunction); // 12 32 45 71(26 33 53 80)

  // print out content:
  std::cout << "myvector contains:";
  for (int * it= &myints[0]; it!= &myints[8]; ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  // using object as comp
  std::sort (myvector.begin(), myvector.end(), myobject);     //(12 26 32 33 45 53 71 80)


  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  return 0;
}
