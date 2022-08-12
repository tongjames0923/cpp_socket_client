//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_POINTERABLE_H
#define SOCKET_CLIENT_POINTERABLE_H

#include <memory>





template<typename IMP>
class Pointerable:public virtual std::enable_shared_from_this<Pointerable<IMP>>
{
public:
    operator IMP()
    {
        if (Pointerable<IMP>::alive(*this))
        return this->shared_from_this();
        else
            throw std::runtime_error("Pointerable is not alive");
    }
    virtual ~Pointerable()=default;
    Pointerable()=default;
    static void resetCore(Pointerable<IMP> *self,IMP* imp= nullptr)
    {
        self->m_impl.reset(imp);
    }
    static void makeAlive(Pointerable<IMP> *self)
    {
        self->resetCore(self,new IMP());
    }
    static bool alive(const Pointerable<IMP> &self)
    {
        return self.m_impl.get() != nullptr;
    }
private:
    std::shared_ptr<IMP> m_impl = nullptr;
protected:

    IMP& myImpl()
    {
        Pointerable<IMP> &self =*this;
        if (!Pointerable<IMP>::alive(self))
            throw std::runtime_error("impl is nullptr");
        return *m_impl;
    }
    const IMP& cMyImpl() const
    {
        const Pointerable<IMP> &self =*this;
        if (!Pointerable<IMP>::alive(self))
            throw std::runtime_error("impl is nullptr");

        return *m_impl;
    }
};

template<typename IMP>
class AutoAlivePointerable : public virtual Pointerable<IMP>
{
public:
    AutoAlivePointerable()
    {
        Pointerable<IMP>::makeAlive(this);
    }
    virtual ~AutoAlivePointerable()=default;
};


class None_Copyable
{
public:
    None_Copyable(const None_Copyable &other) = delete;
    None_Copyable()= default;
    virtual  ~None_Copyable()=default;
};

#endif //SOCKET_CLIENT_POINTERABLE_H
