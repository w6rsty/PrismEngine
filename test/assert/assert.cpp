#include "core/assert.hpp"

int main() {
    PRISM_ASSERT(false, "Hello", prism::log_tag::Core);
}