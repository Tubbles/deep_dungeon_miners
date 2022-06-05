#include <cstdint>
#include <fmt/format.h>
#include <string>

#define dfmt(s) fmt::print("[DEBUG] {} ({}:{}): {}", __FUNCTION__, __FILE__, __LINE__, s);
#define wfmt(s) fmt::print("[WARNI] {} ({}:{}): {}", __FUNCTION__, __FILE__, __LINE__, s);
#define efmt(s) fmt::print("[ERROR] {} ({}:{}): {}", __FUNCTION__, __FILE__, __LINE__, s);
