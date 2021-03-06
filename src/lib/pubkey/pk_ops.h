/*
* PK Operation Types
* (C) 2010 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_PK_OPERATIONS_H__
#define BOTAN_PK_OPERATIONS_H__

#include <botan/pk_keys.h>
#include <botan/secmem.h>
#include <botan/rng.h>

namespace Botan {

namespace PK_Ops {

template<typename Key>
struct PK_Spec
   {
   public:
      PK_Spec(const Key& key, const std::string& pad) :
         m_key(key), m_pad(pad) {}

      std::string algo_name() const { return m_key.algo_name(); }

      std::string as_string() const { return algo_name() + "/" + padding(); }

      const Key& key() const { return m_key; }
      const std::string& padding() const { return m_pad; }
   private:
      const Key& m_key;
      const std::string m_pad;
   };

/**
* Public key encryption interface
*/
class BOTAN_DLL Encryption
   {
   public:
      virtual size_t max_input_bits() const = 0;

      virtual secure_vector<byte> encrypt(const byte msg[], size_t msg_len, RandomNumberGenerator& rng) = 0;

      typedef PK_Spec<Public_Key> Spec;

      virtual ~Encryption() {}
   };

/**
* Public key decryption interface
*/
class BOTAN_DLL Decryption
   {
   public:
      virtual size_t max_input_bits() const = 0;

      virtual secure_vector<byte> decrypt(const byte msg[],  size_t msg_len) = 0;

      typedef PK_Spec<Private_Key> Spec;

      virtual ~Decryption() {}
   };

/**
* Public key signature creation interface
*/
class BOTAN_DLL Signature
   {
   public:
      /**
      * Find out the number of message parts supported by this scheme.
      * @return number of message parts
      */
      virtual size_t message_parts() const { return 1; }

      /**
      * Find out the message part size supported by this scheme/key.
      * @return size of the message parts
      */
      virtual size_t message_part_size() const { return 0; }

      /**
      * Get the maximum message size in bits supported by this public key.
      * @return maximum message in bits
      */
      virtual size_t max_input_bits() const = 0;

      /*
      * Perform a signature operation
      * @param msg the message
      * @param msg_len the length of msg in bytes
      * @param rng a random number generator
      */
      virtual secure_vector<byte> sign(const byte msg[], size_t msg_len,
                                      RandomNumberGenerator& rng) = 0;

      typedef PK_Spec<Private_Key> Spec;

      virtual ~Signature() {}
   };

/**
* Public key signature verification interface
*/
class BOTAN_DLL Verification
   {
   public:
      /**
      * Get the maximum message size in bits supported by this public key.
      * @return maximum message in bits
      */
      virtual size_t max_input_bits() const = 0;

      /**
      * Find out the number of message parts supported by this scheme.
      * @return number of message parts
      */
      virtual size_t message_parts() const { return 1; }

      /**
      * Find out the message part size supported by this scheme/key.
      * @return size of the message parts
      */
      virtual size_t message_part_size() const { return 0; }

      /**
      * @return boolean specifying if this key type supports message
      * recovery and thus if you need to call verify() or verify_mr()
      */
      virtual bool with_recovery() const = 0;

      /*
      * Perform a signature check operation
      * @param msg the message
      * @param msg_len the length of msg in bytes
      * @param sig the signature
      * @param sig_len the length of sig in bytes
      * @returns if signature is a valid one for message
      */
      virtual bool verify(const byte[], size_t,
                          const byte[], size_t)
         {
         throw Invalid_State("Message recovery required");
         }

      /*
      * Perform a signature operation (with message recovery)
      * Only call this if with_recovery() returns true
      * @param msg the message
      * @param msg_len the length of msg in bytes
      * @returns recovered message
      */
      virtual secure_vector<byte> verify_mr(const byte[],
                                           size_t)
         {
         throw Invalid_State("Message recovery not supported");
         }

      typedef PK_Spec<Public_Key> Spec;

      virtual ~Verification() {}
   };

/**
* A generic key agreement Operation (eg DH or ECDH)
*/
class BOTAN_DLL Key_Agreement
   {
   public:
      /*
      * Perform a key agreement operation
      * @param w the other key value
      * @param w_len the length of w in bytes
      * @returns the agreed key
      */
      virtual secure_vector<byte> agree(const byte w[], size_t w_len) = 0;

      typedef PK_Spec<Private_Key> Spec;

      virtual ~Key_Agreement() {}
   };

}

}

#endif
