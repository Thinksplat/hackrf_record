#ifndef C69BEC73_399F_4909_BA23_38980AA7AFF9
#define C69BEC73_399F_4909_BA23_38980AA7AFF9

#include <vector>

class IDataSource {
    public:
    virtual std::vector<char> GetData() = 0;
};

#endif /* C69BEC73_399F_4909_BA23_38980AA7AFF9 */
