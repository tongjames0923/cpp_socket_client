//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_POINTERABLE_HPP
#define SOCKET_CLIENT_POINTERABLE_HPP

#include <memory>
#include <stdexcept>

/// @brief PIMPL基类
/// @tparam IMP 实现类
/// @tparam DEL 实现类删除器
template<typename IMP, typename DEL=std::default_delete<IMP>>
class Pointerable
{
public:
    /// @brief 自身类型
    using selfType = Pointerable<IMP, DEL>;

    /// @brief 转换成实现类的转换方法
    operator IMP()
    {
        if (Pointerable<IMP>::alive(this))
        {
            IMP imp = myImpl();
            return imp;
        } else
            throw std::runtime_error("Pointerable is not alive");
    }

    Pointerable() = default;

    Pointerable(selfType &&other) noexcept
    {
        // cout<<"moved"<<endl;
        selfType::move(this, std::move(other));
    }

    Pointerable(const selfType &other) noexcept
    {
        //cout<<"copy"<<endl;
        copy(this, other);
    }

    /// @brief 拷贝方法
    /// @param des   拷贝目标
    /// @param src   拷贝来源
    /// @return 
    static void copy(selfType *des, const selfType &src) noexcept
    {
        const IMP &src1 = src.cMyImpl();
        resetCore(des, new IMP(src1));
    }

    /// @brief 移动方法
    /// @param des   移动目标
    /// @param src   移动来源
    /// @return 
    static void move(selfType *des, selfType &&src) noexcept
    {
        IMP *old = src.m_impl.release();
        resetCore(des, old);
        resetCore(&src, nullptr);
    }

    /// @brief 重置实现类的对象
    /// @param self 需要重置的指针
    /// @param imp  重置目标，需是手动分配的指针
    /// @return 
    static void resetCore(selfType *self, IMP *imp = nullptr) noexcept
    {
        if (self->m_impl.get() != imp)
            self->m_impl.reset(imp);
    }

    /// @brief 激活实现类核心，构造一个默认的IMP
    /// @param self 所需激活的类指针
    /// @return 
    static void makeAlive(selfType *self) noexcept
    {
        self->resetCore(self, new IMP());
    }

    /// @brief 激活实现类核心，拷贝构造
    /// @param self 所需激活的类指针
    /// @param cpy  提供拷贝的地址
    /// @return 
    static void makeAlive(selfType *self, const IMP &cpy) noexcept
    {
        self->resetCore(self, new IMP(cpy));
    }

    static void makeAlive(selfType *self, IMP &&cpy) noexcept
    {
        self->resetCore(self, new IMP(cpy));
    }

    /// @brief
    /// @param self 所需测试的类
    /// @return  核心是否激活
    static bool alive(const selfType *self) noexcept
    {
        return self->m_impl.operator bool();
    }

private:
    std::unique_ptr<IMP, DEL> m_impl = nullptr;
protected:
    /// @brief 获取实现对象，若为空,则将defaultItem移动至核心，imp需支持移动构造函数
    /// @param defaultItem 
    /// @return 
    IMP &getImpl_Or(IMP &&defaultItem = IMP()) noexcept
    {
        if (!selfType::alive(this))
        {
            resetCore(this, new IMP(std::move(defaultItem)));
        }
        return *m_impl;
    }

    /// @brief 获取const实现对象，若为空,则将defaultItem移动至核心，imp需支持移动构造函数
    /// @param defaultItem
    /// @return
    const IMP &getcImpl_Or(IMP &&defaultItem = IMP()) const noexcept
    {
        if (!selfType::alive(this))
        {
            resetCore(this, new IMP(std::move(defaultItem)));
        }
        return *m_impl;
    }

    /// @brief 获取实现对象，若未激活会报错
    /// @return 
    IMP &myImpl()
    {
        if (!selfType::alive(this))
            throw std::runtime_error("impl is nullptr");
        return *m_impl;
    }

    /// @brief 获取const实现对象，若未激活会报错
    /// @return 
    const IMP &cMyImpl() const
    {
        if (!selfType::alive(this))
            throw std::runtime_error("impl is nullptr");

        return *m_impl;
    }

    /// @brief 获取实现指针
    /// @return 
    IMP *impl_ptr() noexcept
    {
        return m_impl.get();
    }

    /// @brief 获取const的实现指针
    /// @return 
    const IMP *cImpl_ptr() const noexcept
    {
        return m_impl.get();
    }
};

/// @brief 自动激活的PIMPL基类
/// @tparam IMP 实现类
/// @tparam DEL 实现类的删除器
template<typename IMP, typename DEL=std::default_delete<IMP>>
class AutoAlivePointerable : public virtual Pointerable<IMP, DEL>
{
public:
    AutoAlivePointerable()
    {
        Pointerable<IMP, DEL>::makeAlive(this);
    }
};


class None_Copyable
{
public:
    None_Copyable(const None_Copyable &other) = delete;

    None_Copyable() = default;

    virtual  ~None_Copyable() = default;
};

#endif //SOCKET_CLIENT_POINTERABLE_HPP
