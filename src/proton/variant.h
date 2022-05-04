#ifndef PROTON__VARIANT_H
#define PROTON__VARIANT_H
#include <utility>
#include <memory>
#include <string>
#include <typeinfo>
#include <variant>
#include <vector>
#include <fmt/ranges.h>
#include <proton/packet.h>
#include <proton/utils/common.h>
#include <utils/binary_writer.h>

namespace GTServer {
    enum class VariantType : uint8_t {
        NONE,
        FLOAT,
        STRING,
        VECTOR_2,
        VECTOR_3,
        UNSIGNED_INT,
        INT = 9,
    };

    class variant {
    public:
        using var_type = std::variant<float, std::string, CL_Vec2f, CL_Vec3f, uint32_t, int>;
        variant(const var_type& var) : m_object({ var }) {}
        variant(const float& var) : m_object({ var }) {}
        variant(const std::string& var) : m_object({ var }) {}
        variant(const CL_Vec2f& var) : m_object({ var }) {}
        variant(const CL_Vec3f& var) : m_object({ var }) {}
        variant(const uint32_t& var) : m_object({ var }) {}
        variant(const int& var) : m_object({ var }) {}

        template<typename T>
        T get() const {
            return std::get<T>(m_object);
        }
        std::string get() const {
            return std::get<std::string>(m_object);
        }
        VariantType get_type() const {
            switch (m_object.index())
            {
            case 0:
                return VariantType::FLOAT;
            case 1:
                return VariantType::STRING;
            case 2:
                return VariantType::VECTOR_2;
            case 3:
                return VariantType::VECTOR_3;
            case 4:
                return VariantType::UNSIGNED_INT;
            case 5:
                return VariantType::INT;
            default:
                return VariantType::NONE;
            }
        }
        size_t get_memory_allocate() const {
            size_t alloc{ 0 };
            switch(this->get_type()) {
            case VariantType::STRING: {
                alloc = 5 + this->get().size();
            } break;
            case VariantType::VECTOR_2: {
                alloc = 1 + (2 * sizeof(float));
            } break;
            case VariantType::VECTOR_3: {
                alloc = 1 + (3 * sizeof(float));
            } break;
            case VariantType::FLOAT:
            case VariantType::UNSIGNED_INT:
            case VariantType::INT: {
                alloc = 5;
            } break;
            default:
                break;
            }
            return alloc;
        }

        void pack(binary_writer& buffer) {
            buffer.write<uint8_t>((uint8_t)this->get_type());

            switch (this->get_type()) {
            case VariantType::FLOAT: {
                buffer.write<float>(this->get<float>());
            } break;
            case VariantType::STRING: {
                buffer.write(this->get(), sizeof(uint32_t));
            } break;
            case VariantType::VECTOR_2: {
                const auto& var = this->get<CL_Vec2f>();
                buffer.write<float>(var.x);
                buffer.write<float>(var.y);
            } break;
            case VariantType::VECTOR_3: {
                const auto& var = this->get<CL_Vec3f>();
                buffer.write<float>(var.x);
                buffer.write<float>(var.y);
                buffer.write<float>(var.z);
            } break;
            case VariantType::UNSIGNED_INT: {
                buffer.write<uint32_t>(this->get<uint32_t>());
            } break;
            case VariantType::INT: {
                buffer.write<int>(this->get<int>());
            } break;
            default:
                break;
            }
        }
    private:
        var_type m_object;
    };

    class variantlist_t {
    public:
        variantlist_t(const variant::var_type& v0) 
            : m_objects({ { v0 } }) {}
        variantlist_t(const variant::var_type& v0, const variant::var_type& v1) 
            : m_objects({ { v0 }, { v1 } }) {}
        variantlist_t(const variant::var_type& v0, const variant::var_type& v1, const variant::var_type& v2) 
            : m_objects({ { v0 }, { v1 }, { v2 } }) {}
        variantlist_t(const variant::var_type& v0, const variant::var_type& v1, const variant::var_type& v2, const variant::var_type& v3) 
            : m_objects({ { v0 }, { v1 }, { v2 }, { v3 } }) {}
        variantlist_t(const variant::var_type& v0, const variant::var_type& v1, const variant::var_type& v2, const variant::var_type& v3, const variant::var_type& v4) 
            : m_objects({ { v0 }, { v1 }, { v2 }, { v3 }, { v4 } }) {}
        variantlist_t(const variant::var_type& v0, const variant::var_type& v1, const variant::var_type& v2, const variant::var_type& v3, const variant::var_type& v4, const variant::var_type& v5) 
            : m_objects({ { v0 }, { v1 }, { v2 }, { v3 }, { v4 }, { v5 } }) {}
        variantlist_t(const variant::var_type& v0, const variant::var_type& v1, const variant::var_type& v2, const variant::var_type& v3, const variant::var_type& v4, const variant::var_type& v5, const variant::var_type& v6) 
            : m_objects({ { v0 }, { v1 }, { v2 }, { v3 }, { v4 }, { v5 }, { v6 } }) {}

        [[nodiscard]] uint8_t* serialize() const {
            size_t alloc = 1;
            for(const auto& var : m_objects)
                alloc += var.get_memory_allocate() + 1;
            binary_writer buffer(alloc);
            buffer.write<uint8_t>(m_objects.size());
            for (size_t index = 0; index < m_objects.size(); index++) {
                variant var = m_objects[index];
                buffer.write<uint8_t>(index);
                var.pack(buffer);
            }
            uint8_t* ret = (uint8_t*)std::malloc(alloc);
            memcpy(ret, buffer.get(), alloc);
            return ret;
        }
        [[nodiscard]] std::vector<variant> get_objects() const { return m_objects; }
    private:
        std::vector<variant> m_objects{};
    };
}

#endif // PROTON__VARIANT_H