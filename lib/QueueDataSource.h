#ifndef B43490AA_4B11_470C_A740_4A7074497ED3
#define B43490AA_4B11_470C_A740_4A7074497ED3

#include "IDataSource.h"
#include "queue.h"

class QueueDataSource : public IDataSource
{
public:
    QueueDataSource(BufferQueue &queue) : m_queue(queue) {}
    virtual std::vector<char> GetData()
    {
        return m_queue.pop();
    }

private:
    BufferQueue &m_queue;
};

#endif /* B43490AA_4B11_470C_A740_4A7074497ED3 */
