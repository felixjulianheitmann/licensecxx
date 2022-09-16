#ifndef LCXX__LCXX_LICENSE_HPP__
#define LCXX__LCXX_LICENSE_HPP__

#include <concepts>
#include <cstddef>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

namespace lcxx {

    using signature = std::string;

    /**
     * @brief A general license data class
     * This class describes a data objects that holds arbitrary key-value string parameters and a signature
     */
    class license {

    public:
        license( std::unordered_map< std::string, std::string > const & content ) : content_( content ) {}
        license() : content_() {}

        /**
         * @brief Adds content to the license. This collection of key-value pairs will be incorporated into the
         * signature
         *
         * @param key
         * @param value
         * @return true if the key has not existed before and was inserted with a new value
         * @return false if the key has existed already and been overwritten with a new value
         */
        auto push_content( std::string const & key, std::string const & value ) -> bool
        {
            return content_.insert_or_assign( key, value ).second;
        }
        template < std::integral I > auto push_content( std::string const & key, I const value ) -> bool
        {
            return push_content( key, std::to_string( value ) );
        }
        template < std::floating_point F > auto push_content( std::string const & key, F const value ) -> bool
        {
            return push_content( key, std::to_string( value ) );
        }

    private:
        std::unordered_map< std::string, std::string > content_;
    };

}  // namespace lcxx

#endif  // LCXX__LCXX_LICENSE_HPP__