#include <lcxx/hash.hpp>

#include <array>
#include <cstddef>
#include <memory>

namespace {
    struct mdctx_free {
        void operator()( EVP_MD_CTX * md_ctx )
        {
            if ( md_ctx )
                EVP_MD_CTX_free( md_ctx );
        }
    };
}  // namespace

namespace lcxx::hash {

    auto sha512( std::string const & input ) -> std::pair< sha512_hash_t, error >
    {
        std::span< const std::byte > const byte_span = { reinterpret_cast< std::byte const * >( input.data() ),
                                                         input.size() };
        return sha512( byte_span );
    }

    auto sha512( std::span< const std::byte > const input ) -> std::pair< sha512_hash_t, error >
    {
        sha512_hash_t hash_buffer;
        auto          with = [&hash_buffer]( error code ) { return std::make_pair( hash_buffer, code ); };

        auto ctx = std::unique_ptr< EVP_MD_CTX, mdctx_free >( EVP_MD_CTX_new(), mdctx_free{} );
        if ( ctx == nullptr )
            return with( error::ctx_allocation_failure );

        if ( 1 != EVP_DigestInit_ex( ctx.get(), EVP_sha512(), NULL ) )
            return with( error::digest_init_failure );

        if ( 1 != EVP_DigestUpdate( ctx.get(), input.data(), input.size() ) )
            return with( error::digest_update_failure );

        unsigned int len;
        if ( 1 != EVP_DigestFinal_ex( ctx.get(), reinterpret_cast< unsigned char * >( hash_buffer.data() ), &len ) )
            return with( error::digest_finalization_failure );

        return with( error::ok );
    }

}  // namespace lcxx::hash
