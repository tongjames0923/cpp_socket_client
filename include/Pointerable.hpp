//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_POINTERABLE_HPP
#define SOCKET_CLIENT_POINTERABLE_HPP

#include <memory>

template<typename IMP, typename DEL=std::default_delete<IMP>>
class Pointerable
{
public:
    using selfType = Pointerable<IMP, DEL>;

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

    static void copy(selfType *des, const selfType &src) noexcept
    {
        const IMP &src1 = src.cMyImpl();
        resetCore(des, new IMP(src1));
    }

    static void move(selfType *des, selfType &&src) noexcept
    {
        IMP *old = src.m_impl.release();
        resetCore(des, old);
        resetCore(&src, nullptr);
    }

    static void resetCore(selfType *self, IMP *imp = nullptr) noexcept
    {
        if (self->m_impl.get() != imp)
            self->m_impl.reset(imp);
    }

    static void makeAlive(selfType *self) noexcept
    {
        self->resetCore(self, new IMP());
    }
    static void makeAlive(selfType *self,const IMP& cpy) noexcept
    {
        self->resetCore(self, new IMP(cpy));
    }
    static void makeAlive(selfType *self,IMP&& cpy) noexcept
    {
        self->resetCore(self, new IMP(cpy));
    }
    static bool alive(const selfType *self) noexcept
    {
        return self->m_impl.operator bool();
    }

private:

    std::unique_ptr<IMP, DEL> m_impl = nullptr;
protected:

    IMP& getImpl_Or(IMP&& defaultItem) noexcept
    {
        if (!selfType::alive(this))
        {
            resetCore(this,new IMP(std::move(defaultItem)));
        }
        return *m_impl;
    }
   const IMP& getcImpl_Or(IMP&& defaultItem) const noexcept
    {
        if (!selfType::alive(this))
        {
            resetCore(this,new IMP(std::move(defaultItem)));
        }
        return *m_impl;
    }

    IMP &myImpl()
    {
        if (!selfType::alive(this))
            throw std::runtime_error("impl is nullptr");
        return *m_impl;
    }

    const IMP &cMyImpl() const
    {
        if (!selfType::alive(this))
            throw std::runtime_error("impl is nullptr");

        return *m_impl;
    }

    IMP *saveImpl_ptr() noexcept
    {
        return m_impl.get();
    }

    const IMP *cSaveImpl_ptr() const noexcept
    {
        return m_impl.get();
    }
};

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
