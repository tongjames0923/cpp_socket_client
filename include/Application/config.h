
#ifndef CONFIG_H
#define CONFIG_H
#define RELEASE_MODE
#define VERSION "1.44"

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


#endif
