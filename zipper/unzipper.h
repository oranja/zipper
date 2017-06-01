#pragma once

#include <ctime>
#include <vector>
#include <istream>
#include <ostream>
#include <string>
#include <memory>
#include <map>

namespace zipper {

  class ZipEntry;

  class Unzipper
  {
  public:
    Unzipper(std::istream& buffer);
    Unzipper(std::vector<unsigned char>& buffer);
    Unzipper(const std::string& zipname);
    Unzipper(const std::string& zipname, const std::string& password);

    ~Unzipper(void);

    std::vector<ZipEntry> entries();

    bool extract(const std::string& destination, const std::map<std::string, std::string>& alternativeNames);
    bool extract(const std::string& destination=std::string());
    bool extractEntry(const std::string& name, const std::string& destination = std::string());
    bool extractEntryToStream(const std::string& name, std::ostream& stream);
    bool extractEntryToMemory(const std::string& name, std::vector<unsigned char>& vec);

    void close();

  private:
    std::string m_password;
    std::string m_zipname;
    bool m_open;

    struct Impl;
    Impl* m_impl;
  };


  class ZipEntry
  {
  public:
    ZipEntry(const std::string& name,
      unsigned long long int compressed_size, unsigned long long int uncompressed_size,
      const struct tm &timestamp)
      : name(name), compressedSize(compressed_size), uncompressedSize(uncompressed_size), timestamp(timestamp)
    {
      // timestamp YYYY-MM-DD HH:MM:SS
      static const size_t TS_BUF_SIZE = 20;
      char timestamp_buf[TS_BUF_SIZE] = { 0 };

      std::strftime(timestamp_buf, TS_BUF_SIZE, "%Y-%m-%d %H:%M:%S", &timestamp);
      timestamp_str = std::string(timestamp_buf);
    }

    bool valid() { return !name.empty(); }

    std::string name, timestamp_str;
    unsigned long long int compressedSize, uncompressedSize;
    struct tm timestamp;
  };
}
