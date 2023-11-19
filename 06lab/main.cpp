#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>

enum TMRecordType { LONG = 0, DOUBLE, CODE, POINT };

struct ServiceMessage {
  uint8_t message_type;
  uint8_t value_type;
  union {
    struct {
      uint16_t len;
      uint8_t message;
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
  void read_service_message(ServiceMessage& record) { throw std::logic_error("read_service_message() is not implemented"); }

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
  std::ifstream ifs("06lab/test.KNP", std::ios::in | std::ios::binary);
  if (!ifs) {
    std::cerr << "ifs\n";
    return EXIT_FAILURE;
  }
  TMRecord tm{};
  ifs >> tm;
  std::cout << tm.par_n;
  return EXIT_SUCCESS;
}