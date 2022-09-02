#pragma once
#include <cstdint>
#include <sqlpp11/table.h>
#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>

namespace GTServer {
    namespace world_i {
        struct ID {
            struct _alias_t {
                static constexpr const char _literal[] = "id";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T id;
                    T& operator()() {
                        return id;
                    }

                    const T& operator()() const {
                        return id;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::integral, ::sqlpp::tag::must_not_insert, ::sqlpp::tag::must_not_update>;
        };
        struct Name {
            struct _alias_t {
                static constexpr const char _literal[] = "name";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T name;
                    T& operator()() {
                        return name;
                    }

                    const T& operator()() const {
                        return name;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar, ::sqlpp::tag::must_not_update>;
        };
        struct Width {
            struct _alias_t {
                static constexpr const char _literal[] = "width";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T width;
                    T& operator()() {
                        return width;
                    }

                    const T& operator()() const {
                        return width;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::integer>;
        };
        struct Height {
            struct _alias_t {
                static constexpr const char _literal[] = "height";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T height;
                    T& operator()() {
                        return height;
                    }

                    const T& operator()() const {
                        return height;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::integer>;
        };
        struct CreatedAt {
            struct _alias_t {
                static constexpr const char _literal[] = "created_at";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T created_at;
                    T& operator()() {
                        return created_at;
                    }

                    const T& operator()() const {
                        return created_at;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::time_point>;
        };    
        struct UpdatedAt {
            struct _alias_t {
                static constexpr const char _literal[] = "updated_at";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T updated_at;
                    T& operator()() {
                        return updated_at;
                    }

                    const T& operator()() const {
                        return updated_at;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::time_point>;
        };
        struct Data {
            struct _alias_t {
                static constexpr const char _literal[] = "data";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T data;
                    T& operator()() {
                        return data;
                    }

                    const T& operator()() const {
                        return data;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::blob>;
        };
    }
        
    struct WorldDB 
    : sqlpp::table_t<WorldDB, 
        world_i::ID, 
        world_i::Name, 
        world_i::Width, 
        world_i::Height, 
        world_i::CreatedAt,
        world_i::UpdatedAt, 
        world_i::Data> {
        struct _alias_t {
            static constexpr const char _literal[] = "worlds";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T world_db;
                T& operator()() {
                    return world_db;
                }
                const T& operator()() const {
                    return world_db;
                }
            };
        };
    };
}