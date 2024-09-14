#include <memory>

#include "lyf.h"
#include "stopwatch.h"
#include "lyf_shared_ptr.h"
#include "memCheck.h"

using lyf::PrintTool::print_args;

int main() {
    lyf::auto_stopwatch sw(lyf::stopwatch::TimeType::ms);
    TRACE_OFF();
    MEM_ON();

    MEM_OFF();
    print_args("------------lyf--------------");
    return 0;
}
