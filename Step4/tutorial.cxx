// 一个简单的程序，可以计算数字的平方根
#include <cmath>
#include <iostream>
#include <string>

#include "TutorialConfig.h"

// 我们应该包括MathFunctions标头吗？
#ifdef USE_MYMATH
#  include "MathFunctions.h"
#endif

int main(int argc, char* argv[])
{
  if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }

  // 将输入转换为双精度
  const double inputValue = std::stod(argv[1]);

  // 我们应该使用哪个平方根函数？
#ifdef USE_MYMATH
  const double outputValue = mysqrt(inputValue);
#else
  const double outputValue = sqrt(inputValue);
#endif

  std::cout << "The square root of " << inputValue << " is " << outputValue
            << std::endl;
  return 0;
}
