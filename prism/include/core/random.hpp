#include <random>

namespace prism {
class Random {
public:
    static void Init() {
        s_RandomEngine.seed(std::random_device()());
    }
    static float Float() {
        return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }
    static float UnitFloat() {
        return Float() * 2.0f - 1.0f;
    }
private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

} // namespace prism