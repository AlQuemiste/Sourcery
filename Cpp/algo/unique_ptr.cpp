#include <iostream>
#include <memory> // for std::unique_ptr
 
class Resource
{
public:
  int i = 0;
  Resource() { std::cout << "Resource acquired\n"; }
  ~Resource() { std::cout << "Resource destroyed\n"; }

  friend std::ostream& operator<<(std::ostream& out, const Resource &res)
  {
    out << "I am a resource\n";
    return out;
  }
};

void foo(std::unique_ptr<Resource>& res) {
  if(!res) {
    return;
  }
  
  res->i = 2;
}

int main()
{
  std::unique_ptr<Resource> res1{ new Resource{} }; // Resource created here
  std::unique_ptr<Resource> res2{}; // Start as nullptr
 
  std::cout << "res1 is " << (static_cast<bool>(res1) ? "not null\n" : "null\n");
  std::cout << "res2 is " << (static_cast<bool>(res2) ? "not null\n" : "null\n");
  std::cout << *res1; // print the Resource that res is owning
  // res2 = res1; // Won't compile: copy assignment is disabled
  res2 = std::move(res1); // res2 assumes ownership, res1 is set to null
 
  std::cout << "Ownership transferred\n";
 
  std::cout << "res1 is " << (static_cast<bool>(res1) ? "not null\n" : "null\n");
  std::cout << "res2 is " << (static_cast<bool>(res2) ? "not null\n" : "null\n");

  foo(res2);
  
  return 0;
} // Resource destroyed here when res2 goes out of scope
