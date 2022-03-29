#ifndef D973859B_F679_4191_BD8A_E950B49D2843
#define D973859B_F679_4191_BD8A_E950B49D2843

// include mutex and queue files
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>

template <typename T>
class ThreadsafeQueue
{
public:
    ThreadsafeQueue() {}
    ~ThreadsafeQueue() {}

    void push(const T &value) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(value);
        lock.unlock();
        m_condition.notify_one();
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty()) {
            m_condition.wait(lock);
        }
        T value = m_queue.front();
        m_queue.pop();
        return value;
    }
    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_condition;
};

typedef ThreadsafeQueue<std::vector<char>*> BufferQueue;

#endif /* D973859B_F679_4191_BD8A_E950B49D2843 */
