#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>

// template<typename T>
// auto swap_bytes(T) -> T;

// template <>
// uint16_t swap_bytes(uint16_t v) { return (v << 8) | (v >> 8); }
// template <>
// uint32_t swap_bytes(uint32_t v) {
//   return swap_bytes<uint16_t>(v >> 16) | swap_bytes<uint16_t>(v & 0xffff) <<
//   16;
// }

// enum TMRecordType {
//   LONG = 0, DOUBLE, CODE, POINT
// };

#pragma pack(push, 1)
struct ServiceMessage {
  uint8_t message_type;
  uint8_t value_type;
  union {
    struct {
      uint8_t _dummy[2];
      uint16_t len;
      uint8_t message;
    } session_begin;
    struct {
      uint32_t time;
      uint8_t _dummy[4];
    } current_time;
    struct {
      uint8_t _dummy[4];
      char end[4];
    } session_end;
    struct {
      uint8_t _dummy[4];
      uint32_t mode_n;
    } mode_change;
    struct {
      uint8_t _dummy[4];
      uint32_t error_n;
    } error;
    uint8_t _dummy[8];
  };
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PlainMessage {
  uint8_t dim;
  uint8_t attr : 4;
  uint8_t type : 4;
  union {
    struct {
      uint8_t _dummy[4];
      int32_t value;
    } long_;
    struct {
      double value;
    } double_;
    struct {
      uint8_t _dummy[2];
      uint16_t length;
      int32_t value;
    } code;
    struct {
      uint16_t size;
      uint16_t length;
      uint8_t bytes[4];
    } point;
  };
};
#pragma pack(pop)

struct TMRecord {
  static_assert(sizeof(ServiceMessage) == sizeof(PlainMessage),
                "ServiceMessage should have same size as PlainMessage");
  static_assert(alignof(ServiceMessage) == alignof(PlainMessage),
                "ServiceMessage should have same aligment as PlainMessage");
  inline static constexpr uint16_t SERVICE_TAG = 0xffff;

  uint16_t par_n;
  uint32_t time_ms;
  union {
    ServiceMessage service;
    PlainMessage plain;
  } message;

  // static TMRecord fromBytes(const uint8_t* buff) {
  //   // TMRecord rec(*reinterpret_cast<const TMRecord*>(buff));
  //   // rec.par_n = swap_bytes(rec.par_n);
  //   // rec.time_ms = swap_bytes(rec.time_ms);
  //   // return rec;
  //   return {};
  // }
};

// std::istream& operator>>(std::istream& is, TMRecord& rec) {
//   std::uint8_t buff[sizeof(TMRecord)];
//   is >> buff;
//   // rec = std::move(TMRecord::fromBytes(buff));
//   // if (rec.type == TMRecordType::POINT) {
//   //   const uint16_t size = swap_bytes(*static_cast<uint16_t*>(&buff[10]));
//   // }
//   return is;
// }

auto main() -> int {
  // std::ifstream ifs("06lab\\190829_v29854.KNP", std::ios::in |
  // std::ios::binary); if (!ifs) {
  //   std::cerr << "ifs\n";
  //   return EXIT_FAILURE;
  // }
  // TMRecord tm;
  // ifs >> tm;
  // std::cout << tm.par_n;
  return EXIT_SUCCESS;
}