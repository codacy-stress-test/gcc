// PR c++/89973
// { dg-do compile { target c++14 } }

constexpr int a();

template <typename>
constexpr void *b = a(); // { dg-error "invalid conversion" }
