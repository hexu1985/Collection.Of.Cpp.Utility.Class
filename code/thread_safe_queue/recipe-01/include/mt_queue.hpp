/**
 * @file mt_queue.hpp
 * @brief 一个支持多线程的队列类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-10
 *
 * @see c++并发编程实战, 第4章, queue \n
 * https://github.com/subjam/concurrency-in-action
 */
#ifndef MT_QUEUE_INC
#define MT_QUEUE_INC

#include <mutex>
#include <condition_variable>
#include <queue>

/**
 * @brief 线程安全队列
 *
 * @tparam T 队列元素类型
 */
template <typename T>
class MTQueue {
private:
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<T> queue_;

public:
    MTQueue() = default;

    MTQueue(const MTQueue& other) = delete;

    /**
     * @brief 往队列里放入一个元素
     *
     * @param new_value 元素值
     */
    void push(const T &new_value)
    {
        std::lock_guard<std::mutex> lk(mtx_);
        queue_.push(new_value);
        cv_.notify_one();
    }

    /**
     * @brief 往队列里放入一个元素
     *
     * @param new_value 元素值
     *
     * @note 右值引用类型
     */
    void push(T &&new_value)
    {
        std::lock_guard<std::mutex> lk(mtx_);
        queue_.push(std::forward<T>(new_value));
        cv_.notify_one();
    }

    /**
     * @brief 从队列里取出一个元素
     *
     * @param value 取出元素赋值给该引用
     *
     * @warning 如果队列为空, 该操作会一直阻塞到队列不为空止
     */
    void pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mtx_);
        cv_.wait(lk, [&]{return !queue_.empty();});
        /*
           while (queue_.empty()) {
           cv_.wait(lk);
           }
           */
        value = std::move(queue_.front());
        queue_.pop();
    }

    /**
     * @brief 从队列里取出一个元素
     *
     * @return 取出的元素值
     *
     * @warning 如果队列为空, 该操作会一直阻塞到队列不为空止
     */
    T pop()
    {
        T val;
        pop(val);
        return val;
    }

    /**
     * @brief 从队列里取出一个元素
     *
     * @param value 取出元素赋值给该引用
     *
     * @return 如果队列不为空, 返回true, 否则返回false
     */
    bool tryPop(T &value)
    {
        std::lock_guard<std::mutex> lk(mtx_);
        if (queue_.empty()) 
            return false;
        value = std::move(*queue_.front());
        queue_.pop();
    }

    /**
     * @brief 判断队列是否为空
     *
     * @return 如果队列为空, 返回true, 否则返回false
     */
    bool isEmpty() const
    {
        std::lock_guard<std::mutex> lk(mtx_);
        return queue_.empty();
    }

    /**
     * @brief 获取队列当前元素个数
     *
     * @return 队列当前元素个数
     */
    size_t size() const
    {
        std::lock_guard<std::mutex> lk(mtx_);
        return queue_.size();
    }
};

#endif
