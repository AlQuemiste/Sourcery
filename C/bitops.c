#include <iostream>
#include <cstdint>

typedef std::uint_fast8_t i8;
const i8 mask0 = 0x1; // hex for 0000 0001
const i8 mask1 = 0x2; // hex for 0000 0010
const i8 mask2 = 0x4; // hex for 0000 0100
const i8 mask3 = 0x8; // hex for 0000 1000
const i8 mask4 = 0x10; // hex for 0001 0000
const i8 mask5 = 0x20; // hex for 0010 0000
const i8 mask6 = 0x40; // hex for 0100 0000
const i8 mask7 = 0x80; // hex for 1000 0000

int main() {
  i8 flags{0b0000'0101}; // 8 bits in size means room for 8 flags
  std::cout << "size i8 = " << sizeof(i8) << std::endl;
  std::cout << "bit 0 is " << ((flags & mask0) ? "on\n" : "off\n");
  std::cout << "bit 1 is " << ((flags & mask1) ? "on\n" : "off\n");
  const i8 mask2_clear = ~mask2;
  i8 res_clear = (~mask2) & flags;
  std::cout << "bit 2 is cleared? " << ((res_clear & mask2) ? "on\n" : "off\n");

  const i8 mask = mask0 | mask2;
  const bool res = ((mask & flags) == mask);
  std::cout << "bit 1 and 0 is " << std::boolalpha << res << '\n';

  return 0;
}
