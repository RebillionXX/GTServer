#pragma once
#include <string>
#include <vector>

namespace GTServer
{
    class BinaryWriter
	{
	public:
		BinaryWriter(uint8_t* data, size_t pos = 0) : m_data(data) {
			this->m_pos = pos;
			this->m_delete_after = false;
		}
		BinaryWriter(size_t len) {
			this->m_pos = 0;
			this->m_len = len;

			m_data = new uint8_t[len];
			std::memset(m_data, 0, len);
		}
		~BinaryWriter() {
			if (this->m_delete_after)
				delete[] m_data;
		}

        template<typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void write(T val) {
            std::memcpy(m_data + m_pos, &val, sizeof(T));
			m_pos += sizeof(T);
        }
		template<typename T, typename std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        void write(T val) {
            std::memcpy(m_data + m_pos, &val, sizeof(T));
			m_pos += sizeof(T);
        }
        void write(const std::string& val, const uint16_t& len_size = 2) {
			if (len_size == 2) {
				uint16_t len = (uint16_t)val.length();

				std::memcpy(m_data + m_pos, &len, len_size);
				std::memcpy(m_data + m_pos + len_size, val.c_str(), len);
				m_pos += len + len_size;
				return;
			}
			uint32_t len = (uint32_t)val.length();

			std::memcpy(m_data + m_pos, &len, len_size);
			std::memcpy(m_data + m_pos + len_size, val.c_str(), len);
			m_pos += len + len_size;
		}
		void write(const char* val, std::size_t len) {
			std::memcpy(m_data + m_pos, val, len);
			m_pos += len;
		}
		void skip(size_t len) {
			this->m_pos += len;
		}

		const uint8_t* get() {
			return m_data;
		}
		const size_t get_pos() {
			return m_pos;
		}
		const size_t get_size() {
			return m_len;
		}
	private:
		uint8_t* m_data;
		size_t m_pos;
		size_t m_len;

		bool m_delete_after{ true };
	};
}