#include <cassert>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <stdexcept>

enum TMRecordType : uint8_t { LONG = 0, DOUBLE, CODE, POINT };
enum TMServiceMessageType : uint8_t { EMPTY = 0, SESSION_BEGIN, CUR_TIME, SESSION_END, MODE_CHANGE, ERROR = 6 };

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
  inline static constexpr uint16_t SERVICE_TAG = 0xffff;

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

  template <typename T, bool _littleEndian = std::is_integral_v<T>,
            std::enable_if<std::is_integral_v<std::decay<T>>>* = nullptr>
  void read(T& v) {
    auto buff = reinterpret_cast<char*>(&v);
    is.read(buff, sizeof(T));
    if constexpr (_littleEndian) {
      const size_t size = sizeof(T);
      for (size_t i = 0; i < size / 2; ++i) std::swap(buff[i], buff[size - 1 - i]);
    }
  }

  void skip(long n) { is.seekg(n, std::ios::cur); }
  long off() const { return is.tellg(); }

 private:
  std::istream& is;
};

class TMRecordReader {
 public:
  explicit TMRecordReader(Stream& stream_) : stream(stream_) {}

  void read(TMRecord& record) {
    stream.read(record.par_n);
    stream.read(record.time_ms);
    if (record.par_n == TMRecord::SERVICE_TAG)
      read_service_message(record.service);
    else
      read_plain_message(record.plain);
  }

 private:
  void read_service_message(ServiceMessage& message) {
    stream.read(message.message_type);
    long off = stream.off();
    stream.read(message.value_type);

    uint8_t buff[4];
    switch (message.message_type) {
      case TMServiceMessageType::EMPTY:
        stream.skip(8);
        break;
      case TMServiceMessageType::SESSION_BEGIN:
        stream.skip(2);
        stream.read(message.session_begin.len);
        stream.read(message.session_begin.board_name);
        stream.read(message.session_begin.board_n);
        stream.read(message.session_begin.coil_n);
        stream.read(message.session_begin.data_name);
        stream.read(message.session_begin.n_ver1);
        stream.read(message.session_begin.n_ver2);
        break;
      case 5:  // Unknown message type. Looks like time
        [[fallthrough]];
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
        throw std::logic_error(std::format("Invalid file format. Can't detect service message type. Got {} at 0x{:x}",
                                           message.message_type, off));
    }
  }

  void read_plain_message(PlainMessage& message) {
    stream.read(message.dim);
    uint8_t buff;
    stream.read(buff);
    long off = stream.off();
    message.attr = buff & 0x0f;
    message.type = buff & 0xf0;

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
        stream.read(message.point.bytes);
        if (message.point.length > 4) stream.skip(message.point.length - 4);
        break;
      default:
        throw std::logic_error(std::format("Invalid file format. Can't detect plain message type. Got {} at 0x{:x}",
                                           int(message.type), off));
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

auto format_plain_value(const PlainMessage& msg) {
  switch (msg.type) {
    case TMRecordType::LONG:
      return std::format("{}", msg.long_);
    case TMRecordType::DOUBLE:
      return std::format("{}", msg.double_);
    case TMRecordType::CODE:
      return std::format("Len={}, Value={}", msg.code.length, msg.code.value);
    case TMRecordType::POINT:
      return std::format("ItemSize={} Length={} Bytes=0x{:X} 0x{:X} 0x{:X} 0x{:X}", msg.point.size, msg.point.length,
                         msg.point.bytes[0], msg.point.bytes[1], msg.point.bytes[2], msg.point.bytes[3]);
    default:
      assert(false);
  }
}

constexpr auto fmt = R"""(Parameter n: {}
Time: {}
Dimension n: {}
Attribute n: {}
Value type: {}
Value: {}
)""";

std::ostream& operator<<(std::ostream& os, const TMRecord& rec) {
  if (rec.par_n == TMRecord::SERVICE_TAG) return (os << "<Service message (no details)>");
  const auto s = std::format(fmt, rec.par_n, rec.time_ms, rec.plain.dim, int(rec.plain.attr), int(rec.plain.type),
                             format_plain_value(rec.plain));
  return (os << s);
}

int run(const std::filesystem::path& path) {
  std::ifstream ifs(path, std::ios::in | std::ios::binary);
  if (!ifs) {
    std::cerr << "Error while opening " << path.string();
    return EXIT_FAILURE;
  }

  TMRecord tm{};
  try {
    while (ifs >> tm) {
      if (tm.par_n == TMRecord::SERVICE_TAG)
        continue;
      std::cout << tm << "\n";
    }
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << '\n';
    std::cerr << std::format("File processing '{}' stopped at 0x{:X}", path.string(), long(ifs.tellg()));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

auto main(int argc, char** argv) -> int {
  std::filesystem::path p = "06lab/190829_v29854.KNP";
  if (argc == 2) p = argv[1];
  return run(p);
}