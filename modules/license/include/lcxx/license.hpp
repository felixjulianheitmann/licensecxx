#ifndef LCXX__LCXX_LICENSE_HPP__
#define LCXX__LCXX_LICENSE_HPP__

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace lcxx {

    constexpr auto signature_key = "signature";
    constexpr auto content_key   = "content";

    template < typename T >
    concept string_convertable = std::is_same_v< T, std::string > || std::is_integral_v< T >() ||
        std::is_floating_point_v< T >() || std::is_enum_v< T >() || std::is_convertible_v< T, std::string >;

    using signature     = std::vector< std::byte >;
    using content_map_t = std::unordered_map< std::string, std::string >;

    /**
     * @brief A general license data class
     * This class describes a data object that holds arbitrary key-value string parameters.
     * Fundamentally, it's a restricted std::unordered_map<std::string, std::string>
     */
    class license {

    public:
        license( std::unordered_map< std::string, std::string > const & content ) : content_( content ) {}
        license() : content_() {}

        /**
         * @brief Adds content to the license. This collection of key-value pairs will be used to generate a signature
         *
         * @tparam S any value that is either a number, string or convertible to a string
         * @param key a key tag
         * @param value the corresponding value
         * @return true if the key has not existed before and was inserted with a new value
         * @return false if the key has existed already and been overwritten with a new value
         */
        template < string_convertable S > auto push_content( std::string const & key, S const & value ) -> bool
        {
            auto func = [this]( std::string const & k, std::string const & v ) {
                return content_.insert_or_assign( k, v ).second;
            };
            if constexpr ( std::is_same< S, std::string >() )
                return func( key, value );
            if constexpr ( std::is_integral< S >() || std::is_floating_point< S >() )
                return func( key, std::to_string( value ) );
            if constexpr ( std::is_enum< S >() )
                return func( key, std::to_string( static_cast< std::underlying_type< S > >( value ) ) );
            if constexpr ( std::is_convertible< S, std::string >() )
                return func( key, std::string{ value } );

            // Should be unreachable
            throw std::runtime_error( "Unexpected type provided." );
        }
        template < string_convertable S > auto push_content( std::pair< std::string, S > const & kv ) -> bool
        {
            return push_content( kv.first, kv.second );
        }

        /**
         * @brief get a const access to the underlying map object
         *
         * @return content_map_t const& the underlying map object
         */
        auto get_content() const -> content_map_t const & { return content_; }

        /**
         * @brief read a single parameter from the map
         *
         * @param key the key to search for
         * @return std::optional<content_map_t::mapped_type> either the value corresponding to `key` or std::nullopt if
         * that key does not exist
         */
        auto get( std::string const & key ) -> std::optional< content_map_t::mapped_type >
        {
            if ( content_.contains( key ) )
                return content_[key];
            else
                return {};
        }

        /**
         * @brief serialize the license to a single string that is reproducable if the contents of the license are equal
         *
         * @return std::string a single string version of the full license content
         */
        auto stringify() const -> std::string
        {
            std::vector< std::pair< std::string, std::string > > sorted_content{ content_.begin(), content_.end() };
            std::ranges::sort( sorted_content );

            std::string tmp;
            for ( auto const & [k, v] : sorted_content ) {
                tmp += k + ":" + v;
            }
            return tmp;
        }

    private:
        content_map_t content_;
    };

}  // namespace lcxx

#endif  // LCXX__LCXX_LICENSE_HPP__