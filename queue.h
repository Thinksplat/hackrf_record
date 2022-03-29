#ifndef D973859B_F679_4191_BD8A_E950B49D2843
#define D973859B_F679_4191_BD8A_E950B49D2843

class ThreadsafeQueue<T>
{
public:
    ThreadsafeQueue();
    ~ThreadsafeQueue();

    void push(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        lock.unlock();
        condition_.notify_one();
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty()) {
            condition_.wait(lock);
        }
        T value = queue_.front();
        queue_.pop();
        return value;
    }
    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_condition;
};

typedef ThreadsafeQueue<const char[]> BufferQueue;

#endif /* D973859B_F679_4191_BD8A_E950B49D2843 */
