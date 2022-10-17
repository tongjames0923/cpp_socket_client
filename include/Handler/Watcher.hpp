//
// Created by abstergo on 2022/9/11.
//

#ifndef TESTER_WATCHER_HPP
#define TESTER_WATCHER_HPP

#include <functional>
#include <mutex>
#include <future>
#include <chrono>

/// @brief 监控频率
enum WatchRate
{
    /// @brief 缓慢
    Slow = 0,
    /// @brief 中速
    Medium = 1,
    /// @brief 快速
    Fast = 2,
    /// @brief 自定义
    Custom = 3
};

template <typename ARGS>
class DataWatcher
{
public:
    using watch_function = std::function<void(ARGS)>;
    using data_get_function = std::function<ARGS()>;
    using Rate=WatchRate;

    /// @brief
    /// @param f 监视方法
    /// @param rt  频率
    /// @param tk  当频率为自定义时有效，时间单位毫秒
    explicit DataWatcher(watch_function f, Rate rt = Medium, size_t tk = stand_tick[1]) : m_watcher(f), m_rate(rt)
    {
        switch (rt)
        {
        case Rate::Slow:
            m_tick = stand_tick[0];
            break;
        case Rate::Medium:
            m_tick = stand_tick[1];
            break;
        case Rate::Fast:
            m_tick = stand_tick[2];
            break;
        default:
            m_tick = tk;
            break;
        }
    }

    /// @brief 异步的方式监视数据
    /// @param getter 数据获取的方法
    void watch_async(data_get_function getter)
    {
        if (getter != nullptr)
        {
            if (keep)
                throw std::runtime_error("had watching");
            keep = true;
            std::thread look(m_async_task, this, getter);
            look.detach();
            return;
        }
        throw std::runtime_error("data getter can not be null");

        // std::async(std::launch::async, m_async_task,this,args...);
    }
    /// @brief 当rate为custom时可用，设置监视间隔
    /// @param tick
    void setCustomRate(size_t tick)
    {
        if (!keep && m_rate == Rate::Custom)
        {
            this->tick = tick;
            return;
        }
        if (keep)
            throw std::runtime_error("can not set rate in watch");
        else
            throw std::runtime_error("can not set rate at custom level");
    }
    /// @brief 停止监视
    void cancel()
    {
        stop_p.set_value();
        keep = false;
    }
    /// @brief 当前是否在监视中
    /// @return
    bool isWatching() const noexcept
    {
        return keep;
    }

private:
    static constexpr const size_t stand_tick[] = {1500, 500, 100};
    Rate m_rate;
    watch_function m_watcher;
    std::mutex m_data_mut;
    std::promise<void> stop_p;
    std::future<void> fp = stop_p.get_future();
    std::atomic_bool keep;
    std::atomic_int m_tick;

    static void m_async_task(DataWatcher<ARGS> *watcher, data_get_function gt)
    {
        while (watcher->isWatching())
        {
            std::future_status status = watcher->fp.wait_for(std::chrono::milliseconds(watcher->m_tick));
            if (status == std::future_status::ready)
            {
                watcher->keep = false;
                break;
            }
            {
                std::lock_guard<std::mutex> g((watcher->m_data_mut));
                watcher->m_watcher(gt());
            }
        }
    }
};

#endif // TESTER_WATCHER_HPP
