#include "Maybe.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
   std::vector<Maybe<int>> v{Maybe<int>(), Maybe<int>(42), Maybe<int>(13)};
   std::vector<Maybe<bool>> w(3, Maybe<bool>());

   constexpr auto is42 = [](int n) { return n == 42 ? true : false; };
   std::transform(v.begin(), v.end(), w.begin(),
                  std::bind(&Maybe<int>::map<decltype(is42)>, std::placeholders::_1, is42));
   for(const Maybe<bool>& m : w)
   {
      std::cout << m << std::endl;
   }
}

