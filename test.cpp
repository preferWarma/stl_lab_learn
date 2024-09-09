#include "lyf.h"
#include "stopwatch.h"
#include "memCheck.h"   // 用于内存泄漏检测, 必须放在所有头文件的最后

using namespace lyf;
using namespace lyf::StringTool;
using namespace lyf::PrintTool;
using namespace std;

// 通用单例模式测试类
class Foo : public Singleton<Foo> {};

// 通用单例模式测试类
class Foo2 {};

void printWithBlueText(const string& str) {
    cout << "\033[1;34m" << str << "\033[0m";
}

int main() {
    TRACE_OFF();
    MEM_ON();

    // 设置计时器
    stopwatch sw(stopwatch::TimeType::ms); // 计时单位设置为ms
    sw.start();

    string str = "hello world!";

    printWithBlueText("--------字符串分割-----------\n");

    // 字符串分割
    auto res = split(str, ' ');
    print_container(res, "\n"); // 容器快捷打印(基于迭代器和operator<<重载)

    printWithBlueText("--------正则表达式匹配-----------\n");

    // 正则表达式匹配
    string pattern = R"(\d{3,4}-\d{7,8})";
    string text = "电话号码: 010-12345678, 021-87654321";
    auto regex_res = regex_match(text, pattern);
    print_container(regex_res, "\n");   // 默认打印整个容器
    print_container(regex_res.begin(), regex_res.end(), "\n"); // 也可以指定迭代器范围打印

    printWithBlueText("-----字符串替换与参数打印--------\n");

    // 字符串替换
    auto res2 = replace_first(str, "world", "lyf");
    auto res3 = replace_last(str, "world", "USTC");
    auto res4 = replace_all(str, "world", "WHU");

    // 参数打印
    set_printDelim("\n"); // 设置分隔符
    set_printDelimPersist(true); // 设置分隔符持久化
    print_args(res2, res3, res4);

    printWithBlueText("------beginWith和endWith--------\n");

    // beginWith和endWith
    cout << std::boolalpha << begin_with(str, "hello") << endl;
    cout << std::boolalpha << begin_with(str, "world") << endl;
    cout << std::boolalpha << end_with(str, "world") << endl;
    cout << std::boolalpha << end_with(str, "world!") << endl;

    printWithBlueText("\n--------通用单例模式-----------\n");

    // 通用单例模式
    // 对于采用单例继承的方式, 两次打印地址相同, 说明是同一个对象, 推荐使用这种方案
    // 此时如果有 auto f1 = Singleton<Foo>::GetInstance();   则编译器会报错阻止拷贝构造，保证单例
    Singleton<Foo>::GetInstance().printAdress();
    Foo::GetInstance().printAdress();
    vector<Foo*> vf;
    for (int i = 0; i < 3; ++i) {
        vf.push_back(&Foo::GetInstance());
    }
    print_container(vf, "\n");   // 三次打印地址一致且与上述两个地址相同, 说明是同一个对象

    // 对于不继承直接使用单例模式的方式, 两次打印地址相同, 说明是同一个对象
    // 但这种方式没有阻止单例的拷贝或赋值, 不推荐使用, 会导致单例失效
    // 例如: auto f2 = Singleton<Foo2>::GetInstance();
    // 此时f2是一个新的实例，对Singleton<Foo2>::GetInstance()的单例进行了拷贝
    cout << &Singleton<Foo2>::GetInstance() << endl;
    cout << &Singleton<Foo2>::GetInstance() << endl;

    printWithBlueText("\n--------通用多参数max和min和范围比较函数-----------\n");

    // 通用多参数max和min和范围比较函数
    cout << max(1, 1.5f, -5.0, 8.8, 'a', 'b') << endl;
    cout << min(1, 1.5f, -5.0, 8.8, 'a', 'b') << endl;

    vector<int> vec = { 1, 2, 3, 4, 5 };
    cout << range_max(vec.begin(), vec.end()) << endl;
    cout << range_min(vec.begin(), vec.end()) << endl;
    cout << range_compare(vec.begin(), vec.end(), [](int a, int b) { return a < b; }) << endl;  // 等价于求最大值

    printWithBlueText("\n--------类型推导宏typeof-----------\n");

    // 类型推导宏typeof
    cout << typeof(Singleton<Foo>::GetInstance()) << endl;

    printWithBlueText("\n------------运行时断言-------------\n");

    try {
        assure(1 == 2, "1 != 2, 断言失败!");
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }

    MEM_OFF();

    printWithBlueText("\n----------------lyf---------------\n");
    cout << "Duration Time: " << sw.duration() << " ms" << endl;

    return 0;
}