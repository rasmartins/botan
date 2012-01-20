/*
* Policies
* (C) 2004-2006 Jack Lloyd
*
* Released under the terms of the Botan license
*/

#ifndef BOTAN_TLS_POLICY_H__
#define BOTAN_TLS_POLICY_H__

#include <botan/tls_magic.h>
#include <botan/x509cert.h>
#include <botan/dl_group.h>
#include <vector>

namespace Botan {

/**
* TLS Policy Base Class
* Inherit and overload as desired to suite local policy concerns
*/
class BOTAN_DLL TLS_Policy
   {
   public:
      /*
      * Return allowed ciphersuites, in order of preference
      */
      std::vector<u16bit> ciphersuite_list(bool have_srp) const;

      u16bit choose_suite(const std::vector<u16bit>& client_suites,
                          bool have_rsa,
                          bool have_dsa,
                          bool have_srp) const;

      byte choose_compression(const std::vector<byte>& client_algos) const;

      std::vector<std::string> allowed_ciphers() const;

      std::vector<std::string> allowed_hashes() const;

      std::vector<std::string> allowed_key_exchange_methods() const;

      std::vector<std::string> allowed_signature_methods() const;

      virtual std::vector<byte> compression() const;

      virtual bool check_cert(const std::vector<X509_Certificate>& cert_chain) const = 0;

      /**
      * If client authentication is desired, returns a list of allowable
      * CAs for same. If not desired, returns empty list.
      */
      virtual std::vector<X509_Certificate> client_auth_CAs() const
         { return std::vector<X509_Certificate>(); }

      /**
      * Require support for RFC 5746 extensions to enable
      * renegotiation.
      *
      * @warning Changing this to false exposes you to injected
      * plaintext attacks.
      */
      virtual bool require_secure_renegotiation() const { return true; }

      /**
      * Return the group to use for ephemeral Diffie-Hellman key agreement
      */
      virtual DL_Group dh_group() const { return DL_Group("modp/ietf/1536"); }

      /*
      * @return the minimum version that we will negotiate
      */
      virtual Version_Code min_version() const { return SSL_V3; }

      /*
      * @return the version we would prefer to negotiate
      */
      virtual Version_Code pref_version() const { return TLS_V12; }

      virtual ~TLS_Policy() {}
   };

}

#endif
