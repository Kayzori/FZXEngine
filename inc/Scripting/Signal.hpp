#include <vector>

template <typename... Args>
class Signal {
public:
    using SignalSlot = void(*)(Args...);

    void connect(SignalSlot func) {
        if (std::find(Slots.begin(), Slots.end(), func) == Slots.end()) {
            Slots.push_back(func);
        }
    }

    void disconnect(SignalSlot func) {
        Slots.erase(std::remove(Slots.begin(), Slots.end(), func), Slots.end());
    }

    void emit(Args... args) {
        for (SignalSlot& s : Slots) {
            s(args...);
        }
    }

private:
    std::vector<SignalSlot> Slots;
};
