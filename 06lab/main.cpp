#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <cassert>
#include <stdexcept>

enum TMRecordType : uint8_t { LONG = 0, DOUBLE, CODE, POINT };
enum TMServiceMessageType : uint8_t {
  EMPTY = 0,
  SESSION_BEGIN,
  CUR_TIME,
  SESSION_END,
  MODE_CHANGE,
  ERROR = 6
};

struct ServiceMessage {
  uint8_t message_type;
  uint8_t value_type;
  union {
    struct {
      uint16_t len;
      char board_name[4];
      uint8_t board_n;
      char coil_n[5];
      char data_name[8];
      uint8_t n_ver1;
      uint8_t n_ver2;
    } session_begin;
    uint32_t current_time;
    char session_end[4];
    uint32_t mode_n;
    uint32_t error_n;
  };
};

struct PlainMessage {
  uint8_t dim;
  uint8_t attr : 4;
  uint8_t type : 4;
  union {
    int32_t long_;
    double double_;
    struct {
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

struct TMRecord {
  uint16_t par_n;
  uint32_t time_ms;
  union {
    ServiceMessage service;
    PlainMessage plain;
  };
};

class Stream {
 public:
  explicit Stream(std::istream& is_) : is(is_) {}

  template <class T>
  void read(T& buff) {
    is >> buff;
  }
  template <>
  void read(uint8_t& v) {
    is >> v;
  }
  template <>
  void read(uint16_t& v) {
    char buff[2];
    is.read(buff, 2);
    v = (buff[0] << 8) | buff[1];
  }
  template <>
  void read(uint32_t& v) {
    char buff[4];
    is.read(buff, 4);
    v = (buff[0] << 24) | (buff[1] << 16) | (buff[2] << 8) | buff[3];
  }
  void read_arr(char*buff, long n) { is.read(buff, n); }
  
  void skip(long n) { is.seekg(n, std::ios::cur); }

 private:
  std::istream& is;
};

class TMRecordReader {
  inline static constexpr uint16_t SERVICE_TAG = 0xffff;

 public:
  explicit TMRecordReader(Stream& stream_) : stream(stream_) {}

  void read(TMRecord& record) noexcept {
    stream.read(record.par_n);
    stream.read(record.time_ms);
    if (record.par_n == TMRecordReader::SERVICE_TAG)
      read_service_message(record.service);
    else
      read_plain_message(record.plain);
  }

 private:
  void read_service_message(ServiceMessage& message) { 
    stream.read(message.message_type);
    stream.read(message.value_type);
    
    uint8_t buff[4];
    switch (message.message_type) {
      case TMServiceMessageType::EMPTY:
        stream.skip(8);
        break;
      case TMServiceMessageType::SESSION_BEGIN:
        stream.skip(2);
        stream.read(message.session_begin.len);
        stream.read_arr(message.session_begin.board_name, sizeof(message.session_begin.board_name));
        stream.read(message.session_begin.board_n);
        stream.read_arr(message.session_begin.coil_n, sizeof(message.session_begin.coil_n));
        stream.read_arr(message.session_begin.data_name, sizeof(message.session_begin.data_name));
        stream.read(message.session_begin.n_ver1);
        stream.read(message.session_begin.n_ver2);
        break;
      case TMServiceMessageType::CUR_TIME:
        stream.read(message.current_time);
        stream.skip(4);
        break;
      case TMServiceMessageType::SESSION_END:
        stream.skip(4);
        stream.read(buff);
        assert(memcmp(buff, "END!", sizeof(buff)) == 0);
        break;
      case TMServiceMessageType::MODE_CHANGE:
        stream.skip(4);
        stream.read(message.mode_n);
        break;
      case TMServiceMessageType::ERROR:
        stream.skip(4);
        stream.read(message.error_n);
        break;
      default:
        throw std::logic_error("Invalid file format. Can't detect message type.");
    }
  }

  void read_plain_message(PlainMessage& message) {
    stream.read(message.dim);
    uint8_t buff;
    stream.read(buff);
    message.attr = buff & 0xf0;
    message.type = buff & 0x0f;

    switch (message.type) {
    case TMRecordType::LONG:
      stream.skip(4);
      stream.read(message.long_);
      break;
    case TMRecordType::DOUBLE:
      stream.read(message.double_);
      break;
    case TMRecordType::CODE:
      stream.skip(1);
      stream.read(message.code.length);
      stream.read(message.code.value);
      break;
    case TMRecordType::POINT:
      stream.read(message.point.size);
      stream.read(message.point.length);
      for (int i = 0; i < std::min(message.point.length, uint16_t(4)); ++i)
        stream.read(message.point.bytes[i]);
      if (message.point.length > 4)
        stream.skip(message.point.length - 4);
      break;
    default:
      throw std::logic_error("Invalid file format. Can't detect message type.");
    }
  }

 protected:
  Stream& stream;
};

std::istream& operator>>(std::istream& is, TMRecord& rec) {
  Stream stream(is);
  TMRecordReader reader(stream);
  reader.read(rec);
  return is;
}

auto main() -> int {
  std::ifstream ifs("06lab/190829_v29854.KNP", std::ios::in | std::ios::binary);
  if (!ifs) {
    std::cerr << "ifs\n";
    return EXIT_FAILURE;
  }
  TMRecord tm{};
  for( int i= 0; i < 10; ++i) {
    ifs >> tm;
    std::cout << tm.par_n << '\n';
  }
  return EXIT_SUCCESS;
}