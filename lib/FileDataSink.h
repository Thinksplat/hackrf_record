#ifndef BD96A781_420D_49F8_A381_5C2CD15D352F
#define BD96A781_420D_49F8_A381_5C2CD15D352F

#include "IDataSink.h"
#include <fstream>

class FileSink : public IDataSink
{
public:
    FileSink(const char *filename)
    {
        m_file.open(filename, std::ios::out | std::ios::binary);
    }
    virtual ~FileSink()
    {
        m_file.close();
    }
    virtual void PutData(std::vector<char> data)
    {
        PutData(data.data(), data.size());
    }
    virtual void PutData(const char *data, size_t size)
    {
        m_file.write(data, size);
    }
    virtual size_t Size()
    {
        return m_file.tellp();
    }
    virtual void Rewind()
    {
        m_file.seekp(0);
    }

private:
    std::ofstream m_file;
};

#endif /* BD96A781_420D_49F8_A381_5C2CD15D352F */
