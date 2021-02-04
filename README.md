# CMake教程

## 介绍

`CMake` 教程提供了分步指南,其中涵盖了`CMake`可以解决的常见构建系统问题。了解示例项目中各个主题如何协同工作将非常有帮助。教程文档和示例的源代码可以当前项目根目录中找到
。每个步骤都有其自己的子目录,其中包含可以用作起点的代码。教程示例是渐进式的,因此每个步骤都为上一步提供了完整的解决方案。

## 基本起点(步骤1)

最基本的项目是从源代码文件构建的可执行文件。对于简单的项目,只需三行 `CMakeLists.txt` 文件。这将是本教程的起点。在`Step1`目录中创建一个如下所示的`CMakeLists.txt`
文件：

```cmake
cmake_minimum_required(VERSION 3.10)

# 设置项目名
project(Tutorial)

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)
```

请注意,此示例在`CMakeLists.txt`文件中使用小写命令。`CMake` 支持大写,小写和大小写混合命令。 目录中`tutorial.cxx`提供了的源代码,`Step1`可用于计算数字的平方根。

## 添加版本号和配置的头文件

我们将添加的第一个函数是为我们的可执行文件和项目提供版本号。 尽管我们可以仅在源代码中执行此操作,但使用
`CMakeLists.txt` 可以提供更大的灵活性。

首先,修改`CMakeLists.txt`文件以使用：命令：`project`命令设置项目名称和版本号。

然后,配置头文件以将版本号传递给源代码：

由于配置的文件将被写入二进制树,因此我们必须将该目录添加到路径列表中以搜索包含文件。

将以下行添加到 `CMakeLists.txt` 文件的末尾：

使用您喜欢的编辑器,`TutorialConfig.h.in`在源目录中创建以下内容：

当 `CMake` 配置此头文件时, `@Tutorial_VERSION_MAJOR@` 和的值
`@Tutorial_VERSION_MINOR@` 将被替换。

接下来修改 `tutorial.cxx` 以包含配置的头文件
`TutorialConfig.h`。 最后,通过 `tutorial.cxx` 如下更新来打印出可执行文件的名称和版本号 ：

## 指定C++标准

接下来,通过将`in`替换`atof`为
`std::stod`在项目中添加一些`C++ 11`函数`tutorial.cxx`。同时,删除
`#include <cstdlib>`。

我们将依赖在`CMake`代码中明确声明应使用正确的标志。 
在`CMake`中启用对特定`C++`标准的支持的最简单方法是使用`CMAKE_CXX_STANDARD`变量。
对于本教程,将`CMakeLists.txt`
文件中的`CMAKE_CXX_STANDARD`变量设置 为11, 
并将`CMAKE_CXX_STANDARD_REQUIRED`变量设置为`True`。
确保`CMAKE_CXX_STANDARD`在调用上方添加声明`add_executable`。

## 构建和测试

运行：manual：`cmake <cmake(1)>`可执行文件或 ：manual： `cmake-gui <cmake-gui(1)>` 配置项目,
然后使用您选择的构建工具进行构建。
例如,从命令行我们可以切换到根目录并创建一个构建目录：

```shell
 mkdir Step1_build
 ```

接下来,切换到构建目录并运行`CMake`来配置项目并生成本机构建系统：

```shell
cd Step1_build
cmake ../Step2
```

然后调用该构建系统以实际编译/链接项目：

```shell
cmake --build .

```

最后,尝试通过以下命令使用新构建的命令`Tutorial`：

```shell
Tutorial 4294967296
Tutorial 10
Tutorial
```

## 添加库(步骤2)

现在,我们将库添加到我们的项目中。
该库将包含我们自己的实现,用于计算数字的平方根。 
然后可执行文件可以使用此库而不是编译器提供的标准平方根函数。
在本教程中,我们将库放入名为的子目录中 `MathFunctions`。
该目录已经包含一个头文件`MathFunctions.h`和一个源文件`mysqrt.cxx`。 
源文件具有一个称为的 `mysqrt` 函数,该函数提供与编译器`sqrt`函数相似的函数。
将以下一行`CMakeLists.txt`文件添加到`MathFunctions`目录中：
```shell
add_library(MathFunctions mysqrt.cxx)
```
为了使用新的库,我们将 在顶层文件中添加一个： command：`add_subdirectory`,调用顶级 `CMakeLists.txt` 文件,以便构建该库。
我们将新库添加到可执行文件,并添加 `MathFunctions` 为包含目录,以便`mysqrt.h`可以找到头文件。
现在,顶级`CMakeLists.txt`文件的最后几行应如下所示：

```cmake
# 添加MathFunctions库
add_subdirectory(MathFunctions)

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions)

# 将二叉树添加到包含文件的搜索路径
# 这样我们就可以找到TutorialConfig.h
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/MathFunctions"
        )
```

现在让我们将 `MathFunctions` 库设为可选的。
虽然对于本教程而言确实没有任何必要, 但是对于较大的项目,这是常见的情况。 
第一步是向顶层`CMakeLists.txt`文件添加一个选项 。

```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# 配置头文件以传递某些CMake设置
# 到源代码
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

此选项将显示在：manual：`cmake-gui <cmake-gui(1)>`和 ：manual：`ccmake <ccmake(1)>`中 ,默认值`ON`可由用户更改。

此设置将存储在缓存中,因此用户无需在每次在构建目录上运行`CMake`时都设置该值。 
下一个更改是使条件构建和链接`MathFunctions`库成为条件。
为此,我们将顶级`CMakeLists.txt`文件的末尾更改为如下所示：

```cmake
if (USE_MYMATH)
    add_subdirectory(MathFunctions)
    list(APPEND EXTRA_LIBS MathFunctions)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif ()

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# 将二叉树添加到包含文件的搜索路径
# 这样我们就可以找到 TutorialConfig.h
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        ${EXTRA_INCLUDES}
        )
```

请注意,使用变量`EXTRA_LIBS`来收集所有可选库,以便以后链接到可执行文件中。
变量`EXTRA_INCLUDES`类似地用于可选的头文件。

当处理许多可选组件时,这是一种经典方法, 我们将在下一步中,介绍现代方法。

对源代码的相应更改非常简单。首先,如果依赖,请在`tutorial.cxx`中包含`MathFunctions.h`标头：

```c
#ifdef
USE_MYMATH
#  include
"MathFunctions.h"
#endif
```

然后,在同一文件中`,USE_MYMATH`控制使用哪个平方根函数：

```c
#ifdef
USE_MYMATH
const double outputValue = mysqrt(inputValue);
#else
const double outputValue = sqrt(inputValue);
#endif
```

由于源代码现在依赖`USE_MYMATH`,我们可以使用以下行,将其添加到`TutorialConfig.h.in` ：

```c
#cmakedefine USE_MYMATH
```

### 练习：

为什么`TutorialConfig.h.in`在选项之后配置`USE_MYMATH`对我们很重要？如果我们将两者倒置会怎样？

运行：manual：`cmake` <cmake(1)>可执行文件或 ：manual：`cmake-gui` <cmake-gui(
1)> 配置项目,然后使用您选择的构建工具进行构建。
然后运行构建的Tutorial可执行文件。
现在让我们更新的值`USE_MYMATH`。
最简单的方法是在终端中使用 ：manual：`make-gui` <cmake-gui(1)>或
：manual：`ccmake` <ccmake(1)>。或,如果您想从命令行更改选项,请尝试：

```shell
cmake ../Step3 -DUSE_MYMATH=OFF
```

重新生成并再次运行本教程。
`sqrt`或`mysqrt`哪个函数可提供更好的结果？

## 添加库的使用依赖(步骤3)

使用依赖可以更好地控制库或可执行文件的链接并包含行, 同时还可以更好地控制`CMake`内部目标的传递属性。

利用使用需求的主要命令是：

- ：command：`target_compile_definitions`
- ：command：`target_compile_options`
- ：command：`target_include_directories`
- ：target：`link_libraries`

让我们从添加库(第2步)中重构代码,以使用现代的`CMake`使用需求方法。 
我们首先声明,链接到`MathFunctions`的任何人都依赖包含当前源目录, 而`MathFunctions`本身不依赖。
因此这可能成为`INTERFACE`使用依赖。 记
住`INTERFACE`是指消费者依赖的东西,而生产者则不依赖。
将以下行添加到的末尾

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
```

既然我们已经指定了`MathFunction`的使用依赖,我们可以从顶级`CMakeLists.txt`中安全删除对`EXTRA_INCLUDES`变量的使用,这里：

```cmake
if (USE_MYMATH)
    add_subdirectory(MathFunctions)
    list(APPEND EXTRA_LIBS MathFunctions)
endif ()
```

和这里：

```cmake
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        )
```

完成此操作后,运行：manual：`cmake` <cmake(1)>可执行文件或 ：manual：`cmake-gui` <cmake-gui(1)>来配置项目,
然后使用您选择的构建进行构建工具或通过 `cmake --build .` 构建目录使用。

## 安装和测试(步骤4)

现在,我们可以开始为项目添加安装规则和测试支持。

### 安装规则

安装规则非常简单：对于`MathFunctions`,我们要安装库和头文件,
对于应用程序,我们要安装可执行文件和配置的头文件。
因此,于`MathFunctions/CMakeLists.txt`最后,我们添加：

```cmake
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

并在顶层末尾`CMakeLists.txt`添加：

```cmake
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
        DESTINATION include
        )
```

这是创建本教程的基本本地安装依赖的全部。
现在运行：manual：`cmake `<cmake(1)>可执行文件或 ：manual：`cmake-gui` <cmake-gui(1)>来配置项目,
然后使用您选择的构建工具进行构建。
然后运行通过使用安装步骤 `install` 的选项 ：
手动：`cmake`的<cmake的(1)>命令(在3.15引入, 旧版本的 `CMake` 必须使用 `make install` )的命令行。
对于多配置工具,请不要忘记使用`--config`参数指定配置。
如果使用`IDE`,只需构建`INSTALL`
目标。此步骤将安装适当的头文件,库和可执行文件。例如：

```shell
cmake --install .

```

`CMake` 变量 `CMAKE_INSTALL_PREFIX` 用于确定文件的安装根目录。
如果使用该 `cmake --install`命令,则可以通过`--prefix`参数覆盖安装前缀。例如：

```shell
cmake --install . --prefix "/home/myuser/installdir"

```

切换到安装目录,并验证已安装的教程是否运行。

### 测试支持

接下来让我们测试我们的应用程序。在顶级 `CMakeLists.txt`文件的末尾,我们可以启用测试,
然后添加一些基本测试以验证应用程序是否正常运行。

```cmake
enable_testing()

# 应用程序是否运行
add_test(NAME Runs COMMAND Tutorial 25)

# 使用消息有效吗？
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
        PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
        )

# 定义一个函数以简化添加测试
function(do_test target arg result)
    add_test(NAME Comp${arg} COMMAND ${target} ${arg})
    set_tests_properties(Comp${arg}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result}
            )
endfunction(do_test)

# 做一堆基于结果的测试
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is [-nan|nan|0]")
do_test(Tutorial 0.0001 "0.0001 is 0.01")
```

第一个测试只是简单地验证应用程序正在运行,没有段错误或其他崩溃, 并且返回值为零。这是 `CTest` 测试的基本形式。

下一个测试将使用：`prop_test`：`PASS_REGULAR_EXPRESSION`测试属性来验证测试的输出是否包含某些字符串。
在这种情况下,请验证在提供了错误数量的参数时是否打印了用法消息。

最后,我们有一个名为的函数`do_test`, 用于运行应用程序,并验证所计算的平方根对于给定的输入是否正确。 
对于的每次调用`do_test`,都会根据传递的参数将另一个测试连同名称, 输入和预期结果一起添加到项目中。

重建应用程序,然后`cd`到二进制目录并运行 ：manual：`ctest` <ctest(1)>可执行文件：
`ctest -N`和`ctest -VV`。对于多配置生成器(例如`Visual Studio`),必须指定配置类型。
例如,要在'调试' 模式下运行测试,请使用从`build`目录(而非`Debug`子目录！)运行`ctest -C Debug -VV`
。 或者,从 `IDE` 构建目标 `RUN_TESTS` 。

### 添加系统自检(步骤5)

让我们考虑将一些代码添加到我们的项目中, 这取决于目标平台可能不具备的函数。
在此示例中,我们将添加一些代码, 具体取决于目标平台是否具有`log`和`exp`。
当然,几乎每个平台都具有这些函数,但对于本教程而言,它们并不常见。

如果平台具有`log`,`exp`则我们将使用它们来计算函数中的平方根`mysqrt`。
我们首先使用顶级`CMakeLists.txt`中的`CheckSymbolExists`模块测试这些函数的可用性。
在某些平台上,我们将依赖链接到`m`库。
如果最初没有找到`log`和`exp`,则依赖`m`库,然后重试。

我们将在中使用新的定义`TutorialConfig.h.in`,因此请确保在配置该文件之前进行设置。

```cmake
include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if (NOT (HAVE_LOG AND HAVE_EXP))
    unset(HAVE_LOG CACHE)
    unset(HAVE_EXP CACHE)
    set(CMAKE_REQUIRED_LIBRARIES "m")
    check_symbol_exists(log "math.h" HAVE_LOG)
    check_symbol_exists(exp "math.h" HAVE_EXP)
    if (HAVE_LOG AND HAVE_EXP)
        target_link_libraries(MathFunctions PRIVATE m)
    endif ()
endif ()
```

现在,将这些定义添加到中,`TutorialConfig.h.in`以便我们可以从中使用它们`mysqrt.cxx`：

```c
// 平台是否提供exp和log函数？
#cmakedefine HAVE_LOG
#cmakedefine HAVE_EXP
```

如果`log`和`exp`在系统上可用,那么我们将使用它们来计算函数中的平方根`mysqrt`。
将以下代码添加到`MathFunctions/mysqrt.cxx`中的`mysqrt`函数中(返回结果前不要忘记`#endif！`)：

```c
#if
defined(HAVE_LOG) && defined(HAVE_EXP)
double result = exp(log(x)* 0.5);
std::cout << "Computing sqrt of " << x << " to be " << result
<< " using log and exp" << std::endl;
#else
double result = x;
```

我们还依赖修改`mysqrt.cxx`为包含`cmath`。

```c
#include <cmath>
```

运行 `cmake` 可执行文件或 `cmake-gui` 配置项目,然后使用所选的构建工具进行构建,并运行`Tutorial`可执行文件。

您会注意到我们没有使用`log`和`exp`,即使我们认为它们应该可用。 
我们应该很快认识到,我们都忘记了,在`mysqrt.cxx`包含`TutorialConfig.h`。
我们仍需更新,`MathFunctions/CMakeLists.txt`以便`mysqrt.cxx` 知道此文件的位置：

```cmake
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_BINARY_DIR}
        )
```

完成此更新后,继续并再次构建项目,然后运行构建的`Tutorial`可执行文件。
如果`log`和`exp`仍未使用, 请从构建目录打开生成的`TutorialConfig.h`文件。 
也许它们在当前系统上不可用？

`sqrt`或`mysqrt`哪个函数现在可以提供更好的结果？

### 指定编译定义

除了在`TutorialConfig.h`中保存`HAVE_LOG`和`HAVE_EXP`值,我们还有更好的地方吗？

让我们尝试使用`target_compile_definitions()`。

首先,从`TutorialConfig.h.in`中删除定义。我们不再依赖包含`mysqrt.cxx`中的`TutorialConfig.h`或
`MathFunctions/CMakeLists.txt`中的其他包含的内容。

接下来,我们可以将`HAVE_LOG`和`HAVE_EXP`的检查移至`MathFunctions/CMakeLists.txt`,
然后将这些值指定为`PRIVATE`编译定义。

```cmake
include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if (NOT (HAVE_LOG AND HAVE_EXP))
    unset(HAVE_LOG CACHE)
    unset(HAVE_EXP CACHE)
    set(CMAKE_REQUIRED_LIBRARIES "m")
    check_symbol_exists(log "math.h" HAVE_LOG)
    check_symbol_exists(exp "math.h" HAVE_EXP)
    if (HAVE_LOG AND HAVE_EXP)
        target_link_libraries(MathFunctions PRIVATE m)
    endif ()
endif ()

# 添加编译定义
if (HAVE_LOG AND HAVE_EXP)
    target_compile_definitions(MathFunctions
            PRIVATE "HAVE_LOG" "HAVE_EXP")
endif ()
```

完成这些更新后,继续并重新构建项目。运行内置的`Tutorial`可执行文件,并验证结果与本步骤前面的内容相同。

## 添加自定义命令和生成的文件(步骤6)

假设,出于本教程的目的,我们决定不再使用平台`log`和`exp`函数, 而是希望生成一个可在`mysqrt`函数中使用的预计算值表。 
在本节中,我们将在构建过程中创建表,然后将该表编译到我们的应用程序中。
首先,让我们删除 `MathFunctions/CMakeLists.txt` 中对 `log` 和 `exp` 函数的检查。
然后从`mysqrt.cxx`中删除对`HAVE_LOG`和`HAVE_EXP`的检查。
同时,我们可以删除`#include` 。 在`MathFunctions`子目录中,提供了一个名为`MakeTable.cxx`的新源文件来生成表。

查看完文件后,我们可以看到该表是作为有效的C++代码生成的,并且输出文件名作为参数传入。

下一步是将适当的命令添加到`MathFunctions/CMakeLists.txt`文件中, 以构建`MakeTable`可执行文件,然后在构建过程中运行它。

依赖一些命令来完成此操作。

首先,在`MathFunctions/CMakeLists.txt`的顶部,添加`MakeTable`的可执行文件,就像添加任何其他可执行文件一样。

```shell
add_executable(MakeTable MakeTable.cxx)

```

然后,我们添加一个自定义命令,该命令指定如何通过运行`MakeTable`来生成`Table.h`。

```cmake
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
        COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
        DEPENDS MakeTable
)
```

接下来,我们必须让`CMake`知道这`mysqrt.cxx`取决于生成的文件`Table.h`。 
通过将生成的添加`Table.h`到库`MathFunctions`的源列表中来完成此操作。

```cmake
add_library(MathFunctions
        mysqrt.cxx
        ${CMAKE_CURRENT_BINARY_DIR}/Table.h
        )
```

我们还必须将当前的二进制目录添加到包含目录列表中,以便`mysqrt.cxx`可以找到并包含`Table.h`。

```cmake
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )
```

现在,我们使用生成的表。

首先,修改`mysqrt.cxx`以包含`Table.h`。

接下来,我们可以重写`mysqrt`函数以使用该表：

```c
double mysqrt(double x)
{
if (x <= 0) {
return 0;
}

// use the table to help find an initial value
double result = x;
if (x >= 1 && x < 10) {
std::cout << "Use the table to help find an initial value " << std::endl;
result = sqrtTable[static_cast<int>(x)];
}

// do ten iterations
for (int i = 0; i < 10; ++i) {
if (result <= 0) {
result = 0.1;
}
double delta = x - (result* result);
result = result + 0.5* delta/result;
std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
}

return result;
}
```

运行 `cmake` 可执行文件或 `cmake-gui` 配置项目,然后使用所选的构建工具进行构建。

构建此项目时,它将首先构建`MakeTable`可执行文件。然后它将运行`MakeTable`产生`Table.h`。
最后,它将进行编译`mysqrt.cxx`,包含`Table.h`生成`MathFunctions`库。

运行`Tutorial`可执行文件,并验证它是否正在使用该表。

## 构建安装程序(步骤7)

接下来,假设我们想将项目分发给其他人,以便他们可以使用它。
我们希望在各种平台上提供二进制和源代码分发。
这与我们之前在'安装和测试'(第4步)中进行的安装有些不同,
在安装和测试中,我们正在安装根据源代码构建的二进制文件。
在此示例中,我们将构建支持二进制安装和软件包管理函数的安装软件包。
为此,我们将使用`CPack`创建平台特定的安装程序。 
具体来说,我们依赖在顶级`CMakeLists.txt`文件的底部添加几行。

```cmake
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include(CPack)
```

这就是全部。我们首先包含 `InstallRequiredSystemLibraries`。
该模块将包含项目当前平台依赖的任何运行时库。
接下来,我们将一些`CPack`变量设置为存储该项目的许可证和版本信息的位置。
版本信息是在本教程的前面设置的,并且`license.txt`已包含在此步骤的顶级源目录中。

最后,我们将 `CPack module` 它将使用这些变量和当前系统的其他一些属性来设置安装程序。

下一步是按照通常的方式构建项目,然后运行 `cpack` 可执行文件。
要构建二进制发行版,请从二进制目录运行：

```shell
cpack

```

要指定生成器,请使用`-G`选项。对于多配置版本,用于 `-C`指定配置。例如：

```shell
cpack -G ZIP -C Debug
```

要创建源分发,您可以输入：

```shell
cpack --config CPackSourceConfig.cmake

```

或者,运行`make package`或在`IDE`中右键单击`Package`目标和`Build Project`。

运行在二进制目录中找到的安装程序。

然后运行安装的可执行文件并验证其是否有效

## 添加对仪表板的支持(步骤8)

添加将测试结果提交到仪表板的支持非常简单。 
我们已经在'测试支持'中为我们的项目定义了许多测试。
现在,我们只依赖运行这些测试并将其提交到仪表板即可。 
为了包含对仪表板的支持,我们在顶层 `CMakeLists.txt` 中包含了 `CTest` 模块。

用：

```cmake
# enable dashboard scripting
include(CTest)
```

替换：

```cmake
# enable testing
enable_testing()
```

`CTest` 模块将自动调用 `enable_testing()`,因此我们可以将其从 `CMake` 文件中删除。

我们还依赖在顶级目录中创建一个`CTestConfig.cmake`文件,在该目录中我们可以指定项目的名称以及提交仪表板的位置。

```cmake
set(CTEST_PROJECT_NAME "CMakeTutorial")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=CMakeTutorial")
set(CTEST_DROP_SITE_CDASH TRUE)
```

`ctest` 可执行文件将在运行时读入此文件。要创建一个简单的仪表板,您可以运行 `cmake` 可执行文件或 cmake-gui配置项目,
但尚未构建。而,将目录更改为二进制树,然后运行：

```shell
ctest [-VV] -D Experimental

```

请记住,对于多配置生成器(例如`Visual Studio`),必须指定配置类型：

```shell
ctest [-VV] -C Debug -D Experimental

```

或者,从`IDE`中构建`Experimental`目标。

`ctest` 可执行文件将构建并测试项目,并将结果提交到 `Kitware` 的公共仪表板：

https://my.cdash.org/index.php?project=CMakeTutorial

## 混合静态和共享(步骤9)

在本节中,我们将展示如何使用 `BUILD_SHARED_LIBS` 变量来控制 `add_library()` 的默认行为,
并允许控制如何构建没有显式类型(`STATIC`,`SHARED`,`MODULE`或`OBJECT`)的库。

为此,我们依赖添加 `BUILD_SHARED_LIBS` 到顶层`CMakeLists.txt`。我们使用`option()` 命令,
因为它允许用户选择是否将值设置为ON或OFF。

接下来,我们将重构 `MathFunctions` 使其成为使用 `mysqrt` 或封装的真实库 `sqrt` ,
而不是依赖调用代码执行此逻辑。 
这也将意味着 `USE_MYMATH` 它将不控制构建 `MathFunction` ,而将控制该库的行为。

第一步是将顶级 `CMakeLists.txt` 的开始部分更新为：

```cmake

cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Tutorial VERSION 1.0)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 控制静态和共享库的构建位置,以便在Windows上
# 我们不依赖修改运行可执行文件的路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# 配置头文件以仅传递版本号
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 添加 MathFunctions 库
add_subdirectory(MathFunctions)

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC MathFunctions)
```

既然我们已经使 `MathFunctions` 始终被使用,我们将依赖更新该库的逻辑。
因此,在 `MathFunctions/CMakeLists.txt` 中,我们依赖创建一个 `SqrtLibrary`,
当启用 `USE_MYMATH` 时将有条件地构建和安装该 `SqrtLibrary` 。
现在,由于这是一个教程,我们将明确依赖`SqrtLibrary`是静态构建的。

最终结果MathFunctions/CMakeLists.txt应为：

```cmake
# 添加运行的库
add_library(MathFunctions MathFunctions.cxx)

# 声明与我们链接的任何人都依赖包含当前源目录
# 查找MathFunctions.h,而我们找不到。
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

# 我们应该使用自己的数学函数吗
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)

    target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

    # 首先,我们添加生成表的可执行文件
    add_executable(MakeTable MakeTable.cxx)

    # 添加命令以生成源代码
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            DEPENDS MakeTable
    )

    # 只是做 sqrt 的库
    add_library(SqrtLibrary STATIC
            mysqrt.cxx
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            )

    # 声明我们依靠二进制目录找到 Table.h
    target_include_directories(SqrtLibrary PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}
            )

    target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif ()

# 定义在Windows上构建时使用`declspec(dllexport)`的符号
target_compile_definitions(MathFunctions PRIVATE "EXPORTING_MYMATH")

# 安装规则
set(installable_libs MathFunctions)
if (TARGET SqrtLibrary)
    list(APPEND installable_libs SqrtLibrary)
endif ()
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

接下来,为使用`mathfunctions`和 `detail`名称空间，更新`MathFunctions/mysqrt.cxx`：

```c
#include
<iostream>

#include
"MathFunctions.h"

// 包含生成的表
#include
"Table.h"

namespace mathfunctions {
namespace detail {
// 使用简单操作进行 hack 平方根计算
double mysqrt(double x)
{
if (x <= 0) {
return 0;
}

// 使用表格帮助查找初始值
double result = x;
if (x >= 1 && x < 10) {
std::cout << "Use the table to help find an initial value " << std::endl;
result = sqrtTable[static_cast<int>(x)];
}

// 进行十次迭代
for (int i = 0; i < 10; ++i) {
if (result <= 0) {
result = 0.1;
}
double delta = x - (result* result);
result = result + 0.5* delta/result;
std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
}

return result;
}
}
}
```

我们还依赖对中进行一些更改`tutorial.cxx`,以使其不再使用`USE_MYMATH`：

- 一律包含 `MathFunctions.h`

- 一律使用 `mathfunctions::sqrt`

- 不包含`cmath`

最后,更新`MathFunctions/MathFunctions.h`为使用`dll`导出定义：

```c
# if defined(_WIN32)

# if defined(EXPORTING_MYMATH)

# define DECLSPEC __declspec(dllexport)

# else

# define DECLSPEC __declspec(dllimport)

# endif

# else // non windows

# define DECLSPEC

# endif
namespace mathfunctions { double DECLSPEC sqrt(double x); }

```

此时,如果您构建了所有内容,则可能会注意到链接失败,
因为我们将没有位置独立代码的静态库与具有位置独立代码的库组合在一起。
解决方案是显式设置`POSITION_INDEPENDENT_CODE`
无论构建类型如何,`SqrtLibrary` 的 `target` 属性都应为 `True` 。

```cmake
# 声明默认为共享库时，SqrtLibrary 需要 PIC

set_target_properties(SqrtLibrary PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
```
### 练习：

我们修改`MathFunctions.h`为使用`dll`导出定义。
使用`CMake`文档,您可以找到一个帮助器模块来简化此过程吗？

## 添加生成器表达式(步骤10)
`Generator expressions` 在生成系统的过程中进行评估,以生成特定于每个生成配置的信息。

`Generator expressions` 在许多目标属性的上下文中是允许的,
例如 `LINK_LIBRARIES`, `INCLUDE_DIRECTORIES`,
`COMPILE_DEFINITIONS`和别的。
在使用命令填充这些属性时,也可以使用它们,
例如 `target_link_libraries()`, `target_include_directories()`,
`target_compile_definitions()` 和别的。

`Generator expressions` 可能用于启用条件链接,
编译时使用的条件定义,条件包含目录等。
条件可以基于构建配置,目标属性,平台信息或任何其他可查询的信息。

有不同类型的 `generator expressions` 包含逻辑,信息和输出表达式。

逻辑表达式用于创建条件输出。
基本表达式是0和1表达式。
一个`$<0:...>`导致空字符串,
而`<1:...>`导致的内容`...`。它们也可以嵌套。

生成器表达式的常见用法是有条件地添加编译器标志,例如用于语言级别或警告的标志。

一个不错的模式是将该信息与一个接口目标相关联,以允许该信息传播。

首先,我们构建一个 `INTERFACE` 目标并指定依赖的C++标准级别11,而不是使用 `CMAKE_CXX_STANDARD` 。

所以下面的代码：
```cmake

# 指定C ++标准

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

```

## 将被替换为：

```cmake
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)
```
接下来,我们为项目添加依赖的编译器警告标志。由于警告标志根据编译器的不同而不同,
因此我们使用`COMPILE_LANG_AND_ID` 生成器表达式来控制在给定一种语言和一组编译器ID的情况下应用哪些标志,如下所示：

```cmake
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(tutorial_compiler_flags INTERFACE
"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
```
对此,我们看到警告标志封装在 `BUILD_INTERFACE` 条件中。
这样做是为了使我们已安装项目的使用者不会继承我们的警告标志。

### 练习：

修改MathFunctions/CMakeLists.txt,以使所有目标都具有对`tutorial_compiler_flags`的`target_link_libraries()`调用。

## 添加导出配置(步骤11)
在教程的'安装和测试'(第4步)中,我们添加了 `CMake` 的函数,以安装项目的库和头文件。
在 构建安装程序(第7步)期间,我们添加了打包此信息的函数,以便可以将其分发给其他人。

下一步是添加必要的信息,以便其他`CMake`项目可以使用我们的项目,无论是从构建目录,本地安装还是打包时。

第一步是更新我们的 `install(TARGETS)`
命令不仅指定一个,`DESTINATION` 而且还指定一个 `EXPORT`。
该 `EXPORT` 关键字生成并安装包含代码导入从安装树安装命令列出的所有目标的`CMake`的文件。
因此,让我们继续,通过更新`MathFunctions/CMakeLists.txt`中的`install`命令,显式导出`MathFunctions`库,如下所示：

```cmake

set(installable_libs MathFunctions tutorial_compiler_flags)
if(TARGET SqrtLibrary)
list(APPEND installable_libs SqrtLibrary)
endif()
install(TARGETS ${installable_libs} DESTINATION lib EXPORT MathFunctionsTargets)
install(FILES MathFunctions.h DESTINATION include)

install(EXPORT MathFunctionsTargets FILE MathFunctionsTargets.cmake DESTINATION lib/cmake/MathFunctions
)
```
现在我们已经导出了MathFunctions,我们还依赖显式安装生成的`MathFunctionsTargets.cmake`文件。这
是通过在顶层底部添加以下内容来完成的`CMakeLists.txt`：

```cmake
install(EXPORT MathFunctionsTargets
  FILE MathFunctionsTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)
```

此时,您应该尝试运行CMake。如果一切设置正确,您将看到CMake将生成如下错误：

```text
Target "MathFunctions" INTERFACE_INCLUDE_DIRECTORIES property contains path:

"/Users/robert/Documents/CMakeClass/Tutorial/Step11/MathFunctions"

which is prefixed in the source directory.
```
`CMake` 试图说的是,在生成导出信息的过程中,它将导出与当前机器固有联系的路径,并且在其他机器上无效。
解决方案是更新 `MathFunctions` 的` target_include_directories()`
以了解 `INTERFACE` 在构建目录和安装/软件包中使用它时依赖在不同的位置。
这意味着转换 `target_include_directories()` 依赖 `MathFunctions` 看起来像：

```cmake
target_include_directories(MathFunctions INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
$<INSTALL_INTERFACE:include>
)
```
更新后,我们可以重新运行 `CMake` 并确认它不再发出警告。

至此,我们已经正确地包装了`CMake`依赖的目标信息,
但是我们仍然依赖生成一个`MathFunctionsConfig.cmake`使`CMakefind_package()`命令可以找到我们的项目。
因此,让我们继续将新文件添加到项目的顶层
`Config.cmake.in`,其内容如下：

```cmake
@PACKAGE_INIT@

include ( "${CMAKE_CURRENT_LIST_DIR}/MathFunctionsTargets.cmake" )
```
然后,要正确配置和安装该文件,请将以下内容添加到顶层的底部`CMakeLists.txt`：

```cmake

install(EXPORT MathFunctionsTargets FILE MathFunctionsTargets.cmake DESTINATION lib/cmake/MathFunctions
)

include(CMakePackageConfigHelpers)

# 生成包含导出的配置文件

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
"${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake"
INSTALL_DESTINATION "lib/cmake/example"
NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# 生成配置文件的版本文件

write_basic_package_version_file(
"${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfigVersion.cmake"
VERSION "${Tutorial_VERSION_MAJOR}.${Tutorial_VERSION_MINOR}"
COMPATIBILITY AnyNewerVersion
)

# 安装配置文件

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake DESTINATION lib/cmake/MathFunctions
)
```

至此,我们为项目生成了可重定位的`CMake`配置,可以在安装或打包项目后使用它。

如果我们也希望从构建目录中使用我们的项目,则只需将以下内容添加到顶级`CMakeLists.txt`的底部：

```cmake
export(EXPORT MathFunctionsTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsTargets.cmake"
)
```
通过此导出调用,我们现在生成一个`Targets.cmake`,允许`MathFunctionsConfig.cmake`构建目录中的配置供其他项目使用,而无需安装它。

## 打包调试和发布(步骤12)
注意：此示例对单配置生成器有效,而对多配置生成器(例如`Visual Studio`)无效。

默认情况下,`CMake` 的模型是构建目录仅包含单个配置,可以是`Debug`,`Release`,`MinSizeRel`或`RelWithDebInfo`。
但是,可以将 `CPack` 设置为捆绑多个构建目录,并构建一个包含同一项目的多个配置的软件包。

首先,我们要确保调试和发布版本对将要安装的可执行文件和库使用不同的名称。

我们来使用`d`作为调试可执行文件和库的后缀。

```cmake
set(CMAKE_DEBUG_POSTFIX d)
add_library(tutorial_compiler_flags INTERFACE)

```
和 `DEBUG_POSTFIX` 教程可执行文件上的属性：

```cmake
add_executable(Tutorial tutorial.cxx)
set_target_properties(Tutorial PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(Tutorial PUBLIC MathFunctions)
```
我们还要将版本编号添加到 `MathFunctions` 库中。

在 `MathFunctions/CMakeLists.txt` 中,设置 `VERSION` 和 `SOVERSION` 属性：

```cmake
set_property(TARGET MathFunctions PROPERTY VERSION "1.0.0")
set_property(TARGET MathFunctions PROPERTY SOVERSION "1")
```
在 `Step12` 目录中,创建 `debug` 和r `elease` 子目录。布局将如下所示：

- Step12
    - debug
    - release 现在我们依赖设置调试和发布版本。我们可以用 `CMAKE_BUILD_TYPE` 设置配置类型：

```shell
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
cd ../release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```


 现在,调试和发布版本均已完成,我们可以使用自定义配置文件将两个版本打包到一个版本中。
 在 Step12目录中,创建一个名为的文件`MultiCPackConfig.cmake`。
 在此文件中,首先包含由配置文件创建的默认配置文件。 `cmake` 可执行文件。

接下来,使用 `CPACK_INSTALL_CMAKE_PROJECTS` 变量指定要安装的项目。
在这种情况下,我们要同时安装调试和发布。

```cmake
include("release/CPackConfig.cmake")

set(CPACK_INSTALL_CMAKE_PROJECTS
"debug;Tutorial;ALL;/"
"release;Tutorial;ALL;/"
)
```
从 `Step12` 目录运行 `cpack` 使用以下 `config` 选项指定我们的自定义配置文件：

```shell
cpack --config MultiCPackConfig.cmake
```

## the end