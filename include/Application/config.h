
#ifndef CONFIG_H
#define CONFIG_H
#define RELEASE_MODE
#define VERSION "1.48"
#include "Error.hpp"

static constexpr const unsigned long KB = 1024, MB = KB * 1024;
/// @brief 包长
const size_t pack_Len = 32 * KB;
#define IMPL_ASIO

static constexpr const char *SOCKET_IMPL =
#ifdef IMPL_ASIO
        "Implement With ASIO"
#else
"Implement With LibEvent"
#endif // !IMPL_ASIO
;

class PackedData {
private:
    std::unique_ptr<char[]> data = nullptr;
    size_t len = 0;
public:
    explicit PackedData(size_t l) {
        setSize(l);
    }

    explicit operator char *() noexcept {
        return getData();
    }

    void setSize(size_t t) {
        len = t;
        if (t <= 0)
            data.reset();
        else
            data.reset(new char[len]);
    }

    size_t getSize() const noexcept {
        return len;
    }

    char *getData() noexcept {
        return data.get();
    }

    size_t getData(char *des, size_t l) const noexcept {
        size_t t = getSize();
        t = l >= t ? t : l;
        memcpy(des, data.get(), t);
        return t;
    }

};

#endif
