//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_POINTERABLE_HPP
#define SOCKET_CLIENT_POINTERABLE_HPP

#include <memory>
#include <stdexcept>


#define GETTER(type, member) type get##member() const{return member;}
#define SETTER(type, member) void set##member(const type& value){ \
     this->member=value;    \
}
#define PIMPL_IMP_NAME(cn) cn##::imp_##cn

#define PIMPL_DELETER_NAME(cn) cn##::##cn##Deleter


#define PIMPL_INIT(cn)  this->m_impl.reset(new imp_##cn())

#define PIMPL_IMPL(cn) class imp_##cn{

#define PIMPL_IMPL_END };


#define PIMPL_BEGIN_WITH_DELETER(cn) \
class imp_##cn;                      \
class cn##Deleter\
{                                    \
public:                                     \
void operator()(imp_##cn *p) const;\
};                                           \
class cn{ private:  \
std::unique_ptr<imp_##cn,cn##Deleter> m_impl; \


#define PIMPL_END };


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

    Pointerable(const selfType &other)
    {
        //cout<<"copy"<<endl;
        copy(this, other);
    }

    static void copy(selfType *des, const selfType &src)
    {
        if (src.alive(&src))
            resetCore(des, new IMP(*src.m_impl));
        else
        {
            resetCore(des, nullptr);
        }
    }

    static void move(selfType *des, selfType &&src)
    {
        IMP *old = src.m_impl.release();
        resetCore(des, old);
        // resetCore(&src,nullptr);
    }

    static void resetCore(selfType *self, IMP *imp = nullptr)
    {
        if (self->m_impl.get() != imp)
            self->m_impl.reset(imp);
    }

    static void makeAlive(selfType *self)
    {
        self->resetCore(self, new IMP());
    }

    static bool alive(const selfType *self)
    {
        return self->m_impl.operator bool();
    }

private:

    std::unique_ptr<IMP, DEL> m_impl = nullptr;
protected:

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
