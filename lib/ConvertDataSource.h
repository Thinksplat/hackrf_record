#ifndef E30919EB_D6AA_4A8A_8D8C_2525E65BF2EF
#define E30919EB_D6AA_4A8A_8D8C_2525E65BF2EF

#include "IDataSource.h"

class ConvertDataSource : public IDataSource
{
public:
    ConvertDataSource(IDataSource &source) : m_source(source) {}
    virtual std::vector<char> GetData()
    {
        std::vector<char> data = m_source.GetData();
        ConvertSignedToUnsigned(data);
        return data;
    }

    template <class T>
    static void ConvertSignedToUnsigned(T &container)
    {
        char *buf = container.data();
        auto size = container.size();
        // convert each byte to unsigned
        for (unsigned int i = 0; i < size; i++)
        {
            buf[i] ^= 0x80;
        }
    }

private:
    IDataSource &m_source;
};

#endif /* E30919EB_D6AA_4A8A_8D8C_2525E65BF2EF */
