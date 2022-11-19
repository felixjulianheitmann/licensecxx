#include <lcxx/aes.hpp>

#include <algorithm>

namespace lcxx::crypto::aes {

    auto gen_key( salt const salt, std::span< unsigned char const > const key_data, int n_rounds ) -> key_t
    {
        // Generate AES256 key
        auto key = key_t{ new key_struct };

        unsigned char const * salt_ptr = nullptr;
        if ( salt )
            salt_ptr = reinterpret_cast< unsigned char const * >( &salt );

        auto i = EVP_BytesToKey( EVP_aes_256_cbc(), EVP_sha1(), salt_ptr, key_data.data(), key_data.size(), n_rounds,
                                 key->key.data(), key->iv.data() );
        if ( i != key->key.size() ) {
            throw std::runtime_error( "Generating key failed: Size mismatch" );
        }

        return key;
    }

    auto key_to_bytes( key_t const key ) -> std::vector< std::byte >
    {
        std::vector< std::byte > key_bytes( key->key.size() + key->iv.size(), std::byte{} );
        std::ranges::copy( key->key, reinterpret_cast< unsigned char * >( key_bytes.data() ) );
        std::ranges::copy( key->iv, reinterpret_cast< unsigned char * >( key_bytes.data() ) + key->key.size() );
        return key_bytes;
    }

    auto key_from_bytes( std::span< std::byte const > const bytes ) -> key_t
    {
        auto key = key_t{ new key_struct };
        std::copy( reinterpret_cast< unsigned char const * >( bytes.data() ),
                   reinterpret_cast< unsigned char const * >( bytes.data() ) + key->key.size(), key->key.begin() );
        std::copy( reinterpret_cast< unsigned char const * >( bytes.data() ) + key->key.size(),
                   reinterpret_cast< unsigned char const * >( bytes.data() ) + bytes.size(), key->iv.begin() );
        return key;
    }

    auto encrypt( std::span< std::byte const > const input, key_t const key ) -> std::vector< std::byte >
    {
        auto en_ctx     = EVP_CIPHER_CTX_new();
        int  cipher_len = input.size() + AES_BLOCK_SIZE - 1;
        int  final_len  = 0;

        std::vector< std::byte > encrypted( cipher_len );

        EVP_EncryptInit( en_ctx, EVP_aes_256_cbc(), key->key.data(), key->iv.data() );
        EVP_EncryptInit_ex( en_ctx, nullptr, nullptr, nullptr, nullptr );
        EVP_EncryptUpdate( en_ctx, reinterpret_cast< unsigned char * >( encrypted.data() ), &cipher_len,
                           reinterpret_cast< unsigned char const * >( input.data() ), input.size() );
        EVP_EncryptFinal( en_ctx, reinterpret_cast< unsigned char * >( encrypted.data() ) + cipher_len, &final_len );

        encrypted.resize( cipher_len + final_len );
        return encrypted;
    }

    auto decrypt( std::span< std::byte const > const input, key_t const key ) -> std::vector< std::byte >
    {
        auto de_ctx    = EVP_CIPHER_CTX_new();
        int  plain_len = input.size();  // or less
        int  final_len = 0;

        std::vector< std::byte > decrypted( plain_len );

        EVP_DecryptInit( de_ctx, EVP_aes_256_cbc(), key->key.data(), key->iv.data() );
        EVP_DecryptInit_ex( de_ctx, nullptr, nullptr, nullptr, nullptr );
        EVP_DecryptUpdate( de_ctx, reinterpret_cast< unsigned char * >( decrypted.data() ), &plain_len,
                           reinterpret_cast< unsigned char const * >( input.data() ), input.size() );
        EVP_DecryptFinal( de_ctx, reinterpret_cast< unsigned char * >( decrypted.data() ) + plain_len, &final_len );

        decrypted.resize( plain_len + final_len );
        return decrypted;
    }

}  // namespace lcxx::crypto::aes
