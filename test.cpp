#include "lyf.h"
#include "stopwatch.h"

#include "memCheck.h" // 用于内存泄漏检测, 必须放在所有头文件的最后

using namespace lyf;
using namespace lyf::StringTool;
using namespace lyf::PrintTool;

int
main() {
    std::shared_ptr<std::unique_ptr<int>> sp{new std::unique_ptr<int>{new int{1}}};
    sp.get()->reset(new int{2});
    return 0;
}
