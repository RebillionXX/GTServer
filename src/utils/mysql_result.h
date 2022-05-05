#ifndef UTILS__MYSQL_RESULT_H
#define UTILS__MYSQL_RESULT_H

#include <string>
#include <istream>
#include <fmt/core.h>
#include <cppconn/resultset.h>

namespace GTServer
{
    class mysql_result {
    public:
        mysql_result(sql::ResultSet* result) : m_result(result) {}
        ~mysql_result() {
            delete m_result;
        }

        const bool get_bool(const uint32_t& colum_index) {
            return m_result->getInt(colum_index) == 1 ? true : false;
        }
        const bool get_bool(const std::string& colum) {
            return m_result->getInt(colum.c_str()) == 1 ? true : false;
        }
        const uint8_t get_ubyte(const uint32_t& colum_index) {
            return (uint8_t)m_result->getInt(colum_index);
        }
        const uint8_t get_ubyte(const std::string& colum) {
            return (uint8_t)m_result->getInt(colum.c_str());
        }
        const int get_int(const uint32_t& colum_index) {
            return m_result->getInt(colum_index);
        }
        const int get_int(const std::string& colum) {
            return m_result->getInt(colum.c_str());
        }
        const uint32_t get_uint(const uint32_t& colum_index) {
            return (uint32_t)m_result->getInt(colum_index);
        }
        const uint32_t get_uint(const std::string& colum) {
            return (uint32_t)m_result->getInt(colum.c_str());
        }
        const uint64_t get_ulong(const uint32_t& colum_index) {
            return m_result->getUInt64(colum_index);
        }
        const uint64_t get_ulong(const std::string& colum) {
            return m_result->getUInt64(colum.c_str());
        }

        std::string get_string(const std::string& colum) {
            std::istream* val = m_result->getBlob(colum.c_str());
            char val_char[0xFF + 1];
            val->getline(val_char, 0xFF);
            return std::string(val_char);
        }
        std::string get_string(const uint32_t& colum_index) {
            std::istream* val = m_result->getBlob(colum_index);
            char val_char[0xFF + 1];
            val->getline(val_char, 0xFF);
            return std::string(val_char);
        }
        std::vector<uint8_t> get_byte_vector(const uint32_t& colum_index) {
            auto result = m_result->getBlob(colum_index);
            std::vector<uint8_t> buffer;
            std::copy(std::istreambuf_iterator<char>(*result),
                std::istreambuf_iterator<char>(),
                std::back_inserter(buffer));
            return buffer;
        }
        std::vector<uint8_t> get_byte_vector(const std::string& colum) {
            auto result = m_result->getBlob(colum.c_str());
            std::vector<uint8_t> buffer;
            std::copy(std::istreambuf_iterator<char>(*result),
                std::istreambuf_iterator<char>(),
                std::back_inserter(buffer));
            return buffer;
        }
    private:
        sql::ResultSet* m_result;
    };
}
#endif // UTILS__MYSQL_RESULT_H