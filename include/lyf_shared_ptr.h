#ifndef LYF_SHARED_PTR_H_
#define LYF_SHARED_PTR_H_

#include <atomic>
#include <cassert>

#include "lyf_config.h"

NAMESPACE_LYF_BEGIN

// 前置声明
template<class>
class weak_ptr;
template<class>
class enable_shared_from_this;
template<class>
class atomic_shared_ptr;

class shared_weak_count {
    using atomic_t = std::atomic<std::size_t>;

protected:
    atomic_t shared_count_{1}; // 当shared_ptr减小到0后，控制块就会调用被管理对象的析构函数
    atomic_t weak_count_{1};   // weak_count = weak_ref + (shared_count_ != 0), 当weak_count减到0时释放控制块

    // 友元声明
    template<class>
    friend class weak_ptr;
    template<class>
    friend class shared_ptr;
    template<class>
    friend class atomic_shared_ptr;

public:
    shared_weak_count() noexcept = default;
    virtual ~shared_weak_count() = default;

    // 禁止拷贝和赋值
    shared_weak_count(const shared_weak_count&) = delete;
    shared_weak_count&
    operator=(const shared_weak_count&)
        = delete;

    NODISCARD size_t
    use_count() const noexcept { // 获取引用计数
        return shared_count_.load(std::memory_order_relaxed);
    }

    void
    shared_add_ref(const size_t count = 1) noexcept { // 增加共享引用计数
        auto previous = shared_count_++;
        LYF_ASSERT(count == 0 || previous != 0);
    }

    void
    weak_add_ref() noexcept { // 增加弱引用计数
        auto previous = weak_count_++;
        LYF_ASSERT(previous != 0);
    }

    // shared_count为0时调用被管理对象的析构函数
    void
    shared_count_release() noexcept {
        if ((--shared_count_) == 0UL) {
            delete_pointer();
            weak_count_release();
        }
    }

    // weak_count为0时释放整个控制块
    void
    weak_count_release() noexcept {
        if (weak_count_.load(std::memory_order_seq_cst) == 1) {
            delete_control_block();
        } else if ((--weak_count_) == 0UL) {
            delete_control_block();
        }
    }

    // weak_ptr::lock使用
    NODISCARD bool
    increment_if_not_zero() noexcept {
        size_t old_count = shared_count_.load(std::memory_order_relaxed);
        if (old_count == 0) {
            return false;
        }
        while (!shared_count_.compare_exchange_weak(old_count, old_count + 1, std::memory_order_relaxed)) {
            if (old_count == 0) {
                return false;
            }
        }
        return true;
    }

    NODISCARD virtual void*
    get_deleter(const std::type_info&) noexcept {
        return nullptr;
    }

    virtual void
    delete_pointer() noexcept
        = 0;

    virtual void
    delete_control_block() noexcept
        = 0;

    virtual void*
    managed_pointer() const noexcept
        = 0;

}; // class shared_ptr_count

template<class element_type, class Deleter, class Allocator>
class control_block_with_pointer final : public shared_weak_count {
    static_assert(std::is_same<element_type, typename Allocator::value_type>::value, "");

public:
    using pointer        = element_type*;
    using deleter_type   = Deleter;
    using allocator_type = Allocator;

private:
    using alloc_traits               = std::allocator_traits<allocator_type>;
    using control_block_allocator    = typename alloc_traits::template rebind_alloc<control_block_with_pointer>;
    using control_block_alloc_traits = typename alloc_traits::template rebind_traits<control_block_with_pointer>;

    pointer& ptr_;
    deleter_type& deleter_;
    control_block_allocator& allocator_;

public:
    control_block_with_pointer(pointer ptr, deleter_type&& deleter, control_block_allocator&& allocator)
        : ptr_(ptr), deleter_(std::move(deleter)), allocator_(std::move(allocator)) {}

#ifdef LYF_HAS_RTTI
    NODISCARD virtual void*
    get_deleter(const std::type_info& type) noexcept override {
        return (type == typeid(deleter_type)) ? static_cast<void*>(&deleter_) : nullptr;
    }
#endif

    virtual void
    delete_pointer() noexcept override {
        deleter_(ptr_);
        deleter_.~deleter_type();
    }

    virtual void
    delete_control_block() noexcept override {
        auto allocator = std::move(allocator_);
        allocator_.~control_block_allocator();

        control_block_alloc_traits::deallocate(allocator, this, 1);
    }

    virtual void*
    managed_pointer() const noexcept override {
        return ptr_;
    }

}; // class control_block_with_pointer

NAMESPACE_LYF_END /* namespace lyf */

#endif            /* !LYF_SHARED_PTR_H_ */
