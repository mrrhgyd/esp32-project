#ifndef LOCKFREE_QUEUE_HPP
#define LOCKFREE_QUEUE_HPP

#include <atomic>
#include <cstddef>

//定义物理二维坐标点
struct Point2D
{
    float x;
    float y;
};

/**
 * @brief 无锁环形队列 (Lock-Free Ring Buffer)
 * @tparam T 数据类型
 * @tparam Size 队列容量 (必须为 2 的幂以优化取模运算)
 * 
 * 基于单生产者-单消费者模型 (SPSC)：
 * - Core 1 (LVGL/APP_CPU) 作为生产者调用 push()
 * - Core 0 (Pure Pursuit/PRO_CPU) 作为消费者调用 pop()
 * 
 * 使用 memory_order_relaxed + acquire/release 语义实现高效的跨核同步
 */

template<typename T, size_t Size>
class lockfreequeue
{
private:
    //确保 Size 是 2 的幂，方便使用位运算替代取模
    static_assert(Size>0&&(Size&(Size-1))==0,"Size must be a power of 2");
    std::atomic<size_t>head_{0}; //消费者索引
    std::atomic<size_t>tail_{0}; //生产者索引
    T buffer_[Size]{};            //环形缓冲区

public:
    /**
     * @brief Core 1 (LVGL) 调用：推入轨迹点
     * @param item 要推入的元素
     * @return true: 推入成功, false: 队列已满
     * 
     * 性能优化:
     * - memory_order_relaxed: 仅保证原子性，不保证顺序,C++ 原子操作中最弱的内存序
     * - memory_order_release: 确保 buffer 写入在 tail 更新前完成
     */
    bool push(const T& item)
    {
        // 使用位运算快速取模 (Size 为 2^n)
        size_t tail = tail_.load(std::memory_order_relaxed);//原子加载操作
        size_t next_tail = (tail + 1) & (Size - 1);  // 等价于 % Size，但更快
        
        // memory_order_acquire: 确保读到最新的 head，避免读到过期值
        size_t current_head = head_.load(std::memory_order_acquire);//原子加载操作,std::memory_order_acquire提供了acquire 语义（读屏障）
        
        // 检查队列是否已满 (next_tail == head 表示满)
        if (next_tail == current_head) {
            return false;  // 队列满
        }
        
        // 写入数据
        buffer_[tail] = item;
        
        // memory_order_release: 写-写屏障，确保 buffer 写入在 tail 更新前完成
        // 这样消费者 (Core 0) 不会看到更新后的 tail 而读到未写入的数据
        tail_.store(next_tail, std::memory_order_release);//发布‌新写入的数据并更新尾指针
        
        return true;
    }
    /**
     * @brief Core 0 (Pure Pursuit) 调用：取出轨迹点
     * @param item 输出参数，存放取出的元素
     * @return true: 取出成功, false: 队列为空
     * 
     * 性能优化:
     * - memory_order_relaxed: 仅保证原子性
     * - memory_order_acquire: 确保在读取 buffer 前看到最新的 tail
     */
    bool pop(T& item) {
        size_t head = head_.load(std::memory_order_relaxed);
        
        // memory_order_acquire: 确保读到最新的 tail，避免读到过期值
        size_t current_tail = tail_.load(std::memory_order_acquire);
        
        // 检查队列是否为空 (head == tail 表示空)
        if (head == current_tail) {
            return false;  // 队列空
        }
        
        // 读取数据
        item = buffer_[head];
        
        // memory_order_release: 读-读屏障，确保数据读出在 head 更新前完成
        // 这样生产者 (Core 1) 不会看到更新后的 head 而覆盖未读取的数据
        head_.store((head + 1) & (Size - 1), std::memory_order_release);
        
        return true;
    }

    /**
     * @brief 获取当前队列中元素数量 (近似值，用于监控)
     */
    size_t size() const {
        size_t tail = tail_.load(std::memory_order_acquire);
        size_t head = head_.load(std::memory_order_acquire);
        return (tail - head) & (Size - 1);
    }

    /**
     * @brief 清空队列 (仅在确认无并发访问时调用)
     */
    void clear() {
        head_.store(0, std::memory_order_release);
        tail_.store(0, std::memory_order_release);
    }

    /**
     * @brief 检查队列是否为空
     */
    bool is_empty() const {
        return head_.load(std::memory_order_acquire) == 
               tail_.load(std::memory_order_acquire);
    }

    /**
     * @brief 检查队列是否已满
     */
    bool is_full() const {
        size_t tail = tail_.load(std::memory_order_relaxed);
        size_t next_tail = (tail + 1) & (Size - 1);
        return next_tail == head_.load(std::memory_order_acquire);
    }
};

// 无锁队列实例 (延迟初始化，避免全局构造函数在调度器前执行)
// 使用函数内静态变量，首次调用时才构造
inline lockfreequeue<Point2D, 1024>& getTrajectoryQueue() {
    static lockfreequeue<Point2D, 1024> queue;
    return queue;
}
// 宏保持接口兼容
#define g_trajectory_queue (getTrajectoryQueue())






#endif