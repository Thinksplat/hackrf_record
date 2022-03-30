#ifndef C1450D05_E353_4C7B_B436_BC9B7CC16CF6
#define C1450D05_E353_4C7B_B436_BC9B7CC16CF6

class IDataWriter
{
public:
    virtual void PutData(std::vector<char> data) = 0;
    virtual void PutData(const char *data, size_t size) = 0;
};

class IDataSink : public IDataWriter
{
public:
    virtual size_t Size() = 0;
    virtual void Rewind() = 0;
};

#endif /* C1450D05_E353_4C7B_B436_BC9B7CC16CF6 */
