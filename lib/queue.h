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

class BufferSize {
    public:
    BufferSize(int s) {
        bufsize = s;
        
        buffer = new char[s];
    }
    // copy constructor
    BufferSize(const BufferSize &other) {
        bufsize = other.bufsize;
        buffer = other.buffer;
    }
    void Free() {
        delete[] buffer;
    }
    void resize(int s) {
        bufsize = s;
    }
    int size() const 
    {
        return this->bufsize;
    }
    char *data() const
    {
        return buffer;
    }
    protected:

    char *buffer;
    int bufsize;
};

typedef ThreadsafeQueue<BufferSize>
    BufferQueue;

#endif /* D973859B_F679_4191_BD8A_E950B49D2843 */
