#ifndef LYF_CONFIG_H_
#define LYF_CONFIG_H_

// C++标准至少11
#if !defined(__cplusplus) || (__cplusplus < 201103L)
#error "Please use C++ with standard of at least 11"
#endif

// 异常
#ifdef __cpp_exceptions
#define LYF_HAS_EXCEPTIONS
#endif

// 运行时类型信息(RTTI)
#ifdef __cpp_rtti
#define LYF_HAS_RTTI
#endif

// DEBUG 模式
#ifndef NDEBUG
#define LYF_DEBUGGING
#endif

// 减少缩进
#define NAMESPACE_LYF_BEGIN namespace lyf {
#define NAMESPACE_LYF_END   }

// cpp标准版本
#if __cplusplus <= 201103L
#define CXX_STD_VERSION 11
#elif __cplusplus <= 201402L
#define CXX_STD_VERSION 14
#elif __cplusplus <= 201703L
#define CXX_STD_VERSION 17
#elif __cplusplus <= 202002L
#define CXX_STD_VERSION 20
#elif __cplusplus <= 202103L
#define CXX_STD_VERSION 23
#endif // _cpp

// [[nodicard]]
#if CXX_STD_VERSION >= 17
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif

// assert
#ifdef LYF_DEBUGGING
#define ASSERT(cond) assert(cond)
#endif // DEBUG

#define LYF_ASSERT(cond) ASSERT(static_cast<bool>(cond))

#endif /* !LYF_CONFIG_H_ */
