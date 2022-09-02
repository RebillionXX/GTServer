#pragma once
#include <cstdint>
#include <sqlpp11/table.h>
#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>

namespace GTServer {
    namespace account_i {
        struct id {
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

        struct RequestedName {
            struct _alias_t {
                static constexpr const char _literal[] = "requsted_name";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T requsted_name;
                    T& operator()() {
                        return requsted_name;
                    }

                    const T& operator()() const {
                        return requsted_name;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar, ::sqlpp::tag::must_not_update>;
        };

        struct TankIDName {
            struct _alias_t {
                static constexpr const char _literal[] = "tank_id_name";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T tank_id_name;
                    T& operator()() {
                        return tank_id_name;
                    }

                    const T& operator()() const {
                        return tank_id_name;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };

        struct TankIDPass {
            struct _alias_t {
                static constexpr const char _literal[] = "tank_id_pass";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T tank_id_pass;
                    T& operator()() {
                        return tank_id_pass;
                    }

                    const T& operator()() const {
                        return tank_id_pass;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };

        struct RawName {
            struct _alias_t {
                static constexpr const char _literal[] = "raw_name";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T raw_name;
                    T& operator()() {
                        return raw_name;
                    }

                    const T& operator()() const {
                        return raw_name;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };
        
        struct DisplayName {
            struct _alias_t {
                static constexpr const char _literal[] = "display_name";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T display_name;
                    T& operator()() {
                        return display_name;
                    }

                    const T& operator()() const {
                        return display_name;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };
        
        struct Email {
            struct _alias_t {
                static constexpr const char _literal[] = "email";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T email;
                    T& operator()() {
                        return email;
                    }

                    const T& operator()() const {
                        return email;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };
        
        struct Discord {
            struct _alias_t {
                static constexpr const char _literal[] = "discord";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T discord;
                    T& operator()() {
                        return discord;
                    }

                    const T& operator()() const {
                        return discord;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };
        
        struct RelativeIdentifier {
            struct _alias_t {
                static constexpr const char _literal[] = "relative_identifier";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T relative_identifier;
                    T& operator()() {
                        return relative_identifier;
                    }

                    const T& operator()() const {
                        return relative_identifier;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };
        
        struct MachineAddress {
            struct _alias_t {
                static constexpr const char _literal[] = "machine_address";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T machine_address;
                    T& operator()() {
                        return machine_address;
                    }

                    const T& operator()() const {
                        return machine_address;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::varchar>;
        };

        struct Role {
            struct _alias_t {
                static constexpr const char _literal[] = "role";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T role;
                    T& operator()() {
                        return role;
                    }

                    const T& operator()() const {
                        return role;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::integer>;
        };

        struct Inventory {
            struct _alias_t {
                static constexpr const char _literal[] = "inventory";
                using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
                template <typename T>
                struct _member_t {
                    T inventory;
                    T& operator()() {
                        return inventory;
                    }

                    const T& operator()() const {
                        return inventory;
                    }
                };
            };

            using _traits = ::sqlpp::make_traits<::sqlpp::blob>;
        };
    }

    struct Account 
    : sqlpp::table_t<Account, 
        account_i::id, 
        account_i::RequestedName, 
        account_i::TankIDName, 
        account_i::TankIDPass, 
        account_i::RawName,
        account_i::DisplayName, 
        account_i::Email, 
        account_i::Discord, 
        account_i::RelativeIdentifier,
        account_i::MachineAddress,
        account_i::Role,
        account_i::Inventory> {
        struct _alias_t {
            static constexpr const char _literal[] = "players";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T account;
                T& operator()() {
                    return account;
                }
                const T& operator()() const {
                    return account;
                }
            };
        };
    };
}