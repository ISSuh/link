/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_NET_ERROR_H_
#define LINK_NET_BASE_NET_ERROR_H_

#include <cstdint>

namespace nlink {
namespace net {

enum NetError : int32_t {
  OK = 0,
  ERR_IO_PENDING = -1,

  // A generic failure occurred.
  ERR_FAILED = -2,

  // An operation was aborted (due to user action).
  ERR_ABORTED = -3,

  // An argument to the function is incorrect.
  ERR_INVALID_ARGUMENT = -4,

  // The handle or file descriptor is invalid.
  ERR_INVALID_HANDLE = -5,

  // The file or directory cannot be found.
  ERR_FILE_NOT_FOUND = -6,

  // An operation timed out.
  ERR_TIMED_OUT = -7,

  // The file is too large.
  ERR_FILE_TOO_BIG = -8,

  // An unexpected error.  This may be caused by a programming mistake or an
  // invalid assumption.
  ERR_UNEXPECTED = -9,

  // Permission to access a resource, other than the network, was denied.
  ERR_ACCESS_DENIED = -10,

  // The operation failed because of unimplemented functionality.
  ERR_NOT_IMPLEMENTED = -11,

  // There were not enough resources to complete the operation.
  ERR_INSUFFICIENT_RESOURCES = -12,

  // Memory allocation failed.
  ERR_OUT_OF_MEMORY = -13,

  // The file upload failed because the file's modification time was different
  // from the expectation.
  ERR_UPLOAD_FILE_CHANGED = -14,

  // The socket is not connected.
  ERR_SOCKET_NOT_CONNECTED = -15,

  // The file already exists.
  ERR_FILE_EXISTS = -16,

  // The path or file name is too long.
  ERR_FILE_PATH_TOO_LONG = -17,

  // Not enough room left on the disk.
  ERR_FILE_NO_SPACE = -18,

  // The file has a virus.
  ERR_FILE_VIRUS_INFECTED = -19,

  // The client chose to block the request.
  ERR_BLOCKED_BY_CLIENT = -20,

  // The network changed.
  ERR_NETWORK_CHANGED = -21,

  // The request was blocked by the URL block list configured by the domain
  // administrator.
  ERR_BLOCKED_BY_ADMINISTRATOR = -22,

  // The socket is already connected.
  ERR_SOCKET_IS_CONNECTED = -23,

  // The request was blocked because the forced reenrollment check is still
  // pending. This error can only occur on ChromeOS.
  // The error can be emitted by code in chrome/browser/policy/policy_helpers.cc.
  ERR_BLOCKED_ENROLLMENT_CHECK_PENDING = -24,

  // The upload failed because the upload stream needed to be re-read, due to a
  // retry or a redirect, but the upload stream doesn't support that operation.
  ERR_UPLOAD_STREAM_REWIND_NOT_SUPPORTED = -25,

  // The request failed because the URLRequestContext is shutting down, or has
  // been shut down.
  ERR_CONTEXT_SHUT_DOWN = -26,

  // The request failed because the response was delivered along with requirements
  // which are not met ('X-Frame-Options' and 'Content-Security-Policy' ancestor
  // checks and 'Cross-Origin-Resource-Policy', for instance).
  ERR_BLOCKED_BY_RESPONSE = -27,

  // Error -28 was removed (BLOCKED_BY_XSS_AUDITOR).

  // The request was blocked by system policy disallowing some or all cleartext
  // requests. Used for NetworkSecurityPolicy on Android.
  ERR_CLEARTEXT_NOT_PERMITTED = -29,

  // The request was blocked by a Content Security Policy
  ERR_BLOCKED_BY_CSP = -30,

  // The request was blocked because of no H/2 or QUIC session.
  ERR_H2_OR_QUIC_REQUIRED = -31,

  // A connection was closed (corresponding to a TCP FIN).
  ERR_CONNECTION_CLOSED = -100,

  // A connection was reset (corresponding to a TCP RST).
  ERR_CONNECTION_RESET = -101,

  // A connection attempt was refused.
  ERR_CONNECTION_REFUSED = -102,

  // A connection timed out as a result of not receiving an ACK for data sent.
  // This can include a FIN packet that did not get ACK'd.
  ERR_CONNECTION_ABORTED = -103,

  // A connection attempt failed.
  ERR_CONNECTION_FAILED = -104,

  // The host name could not be resolved.
  ERR_NAME_NOT_RESOLVED = -105,

  // The Internet connection has been lost.
  ERR_INTERNET_DISCONNECTED = -106,

  // An SSL protocol error occurred.
  ERR_SSL_PROTOCOL_ERROR = -107,

  // The IP address or port number is invalid (e.g., cannot connect to the IP
  // address 0 or the port 0).
  ERR_ADDRESS_INVALID = -108,

  // The IP address is unreachable.  This usually means that there is no route to
  // the specified host or network.
  ERR_ADDRESS_UNREACHABLE = -109,

  // The server requested a client certificate for SSL client authentication.
  ERR_SSL_CLIENT_AUTH_CERT_NEEDED = -110,

  // A tunnel connection through the proxy could not be established.
  ERR_TUNNEL_CONNECTION_FAILED = -111,

  // No SSL protocol versions are enabled.
  ERR_NO_SSL_VERSIONS_ENABLED = -112,

  // The client and server don't support a common SSL protocol version or
  // cipher suite.
  ERR_SSL_VERSION_OR_CIPHER_MISMATCH = -113,

  // The server requested a renegotiation (rehandshake).
  ERR_SSL_RENEGOTIATION_REQUESTED = -114,

  // The proxy requested authentication (for tunnel establishment) with an
  // unsupported method.
  ERR_PROXY_AUTH_UNSUPPORTED = -115,

  // Error -116 was removed (CERT_ERROR_IN_SSL_RENEGOTIATION)

  // The SSL handshake failed because of a bad or missing client certificate.
  ERR_BAD_SSL_CLIENT_AUTH_CERT = -117,

  // A connection attempt timed out.
  ERR_CONNECTION_TIMED_OUT = -118,

  // There are too many pending DNS resolves, so a request in the queue was
  // aborted.
  ERR_HOST_RESOLVER_QUEUE_TOO_LARGE = -119,

  // Failed establishing a connection to the SOCKS proxy server for a target host.
  ERR_SOCKS_CONNECTION_FAILED = -120,

  // The SOCKS proxy server failed establishing connection to the target host
  // because that host is unreachable.
  ERR_SOCKS_CONNECTION_HOST_UNREACHABLE = -121,

  // The request to negotiate an alternate protocol failed.
  ERR_ALPN_NEGOTIATION_FAILED = -122,

  // The peer sent an SSL no_renegotiation alert message.
  ERR_SSL_NO_RENEGOTIATION = -123,

  // Winsock sometimes reports more data written than passed.  This is probably
  // due to a broken LSP.
  ERR_WINSOCK_UNEXPECTED_WRITTEN_BYTES = -124,

  // An SSL peer sent us a fatal decompression_failure alert. This typically
  // occurs when a peer selects DEFLATE compression in the mistaken belief that
  // it supports it.
  ERR_SSL_DECOMPRESSION_FAILURE_ALERT = -125,

  // An SSL peer sent us a fatal bad_record_mac alert. This has been observed
  // from servers with buggy DEFLATE support.
  ERR_SSL_BAD_RECORD_MAC_ALERT = -126,

  // The proxy requested authentication (for tunnel establishment).
  ERR_PROXY_AUTH_REQUESTED = -127,

  // Error -129 was removed (SSL_WEAK_SERVER_EPHEMERAL_DH_KEY).

  // Could not create a connection to the proxy server. An error occurred
  // either in resolving its name, or in connecting a socket to it.
  // Note that this does NOT include failures during the actual "CONNECT" method
  // of an HTTP proxy.
  ERR_PROXY_CONNECTION_FAILED = -130,

  // A mandatory proxy configuration could not be used. Currently this means
  // that a mandatory PAC script could not be fetched, parsed or executed.
  ERR_MANDATORY_PROXY_CONFIGURATION_FAILED = -131,

  // -132 was formerly ERR_ESET_ANTI_VIRUS_SSL_INTERCEPTION

  // We've hit the max socket limit for the socket pool while preconnecting.  We
  // don't bother trying to preconnect more sockets.
  ERR_PRECONNECT_MAX_SOCKET_LIMIT = -133,

  // The permission to use the SSL client certificate's private key was denied.
  ERR_SSL_CLIENT_AUTH_PRIVATE_KEY_ACCESS_DENIED = -134,

  // The SSL client certificate has no private key.
  ERR_SSL_CLIENT_AUTH_CERT_NO_PRIVATE_KEY = -135,

  // The certificate presented by the HTTPS Proxy was invalid.
  ERR_PROXY_CERTIFICATE_INVALID = -136,

  // An error occurred when trying to do a name resolution (DNS).
  ERR_NAME_RESOLUTION_FAILED = -137,

  // Permission to access the network was denied. This is used to distinguish
  // errors that were most likely caused by a firewall from other access denied
  // errors. See also ERR_ACCESS_DENIED.
  ERR_NETWORK_ACCESS_DENIED = -138,

  // The request throttler module cancelled this request to avoid DDOS.
  ERR_TEMPORARILY_THROTTLED = -139,

  // A request to create an SSL tunnel connection through the HTTPS proxy
  // received a 302 (temporary redirect) response.  The response body might
  // include a description of why the request failed.
  //
  // TODO(https://crbug.com/928551): This is deprecated and should not be used by
  // new code.
  ERR_HTTPS_PROXY_TUNNEL_RESPONSE_REDIRECT = -140,

  // We were unable to sign the CertificateVerify data of an SSL client auth
  // handshake with the client certificate's private key.
  //
  // Possible causes for this include the user implicitly or explicitly
  // denying access to the private key, the private key may not be valid for
  // signing, the key may be relying on a cached handle which is no longer
  // valid, or the CSP won't allow arbitrary data to be signed.
  ERR_SSL_CLIENT_AUTH_SIGNATURE_FAILED = -141,

  // The message was too large for the transport.  (for example a UDP message
  // which exceeds size threshold).
  ERR_MSG_TOO_BIG = -142,

  // Error -143 was removed (SPDY_SESSION_ALREADY_EXISTS)

  // Error -144 was removed (LIMIT_VIOLATION).

  // Websocket protocol error. Indicates that we are terminating the connection
  // due to a malformed frame or other protocol violation.
  ERR_WS_PROTOCOL_ERROR = -145,

  // Error -146 was removed (PROTOCOL_SWITCHED)

  // Returned when attempting to bind an address that is already in use.
  ERR_ADDRESS_IN_USE = -147,

  // An operation failed because the SSL handshake has not completed.
  ERR_SSL_HANDSHAKE_NOT_COMPLETED = -148,

  // SSL peer's public key is invalid.
  ERR_SSL_BAD_PEER_PUBLIC_KEY = -149,

  // The certificate didn't match the built-in public key pins for the host name.
  // The pins are set in net/http/transport_security_state.cc and require that
  // one of a set of public keys exist on the path from the leaf to the root.
  ERR_SSL_PINNED_KEY_NOT_IN_CERT_CHAIN = -150,

  // Server request for client certificate did not contain any types we support.
  ERR_CLIENT_AUTH_CERT_TYPE_UNSUPPORTED = -151,

  // Error -152 was removed (ORIGIN_BOUND_CERT_GENERATION_TYPE_MISMATCH)

  // An SSL peer sent us a fatal decrypt_error alert. This typically occurs when
  // a peer could not correctly verify a signature (in CertificateVerify or
  // ServerKeyExchange) or validate a Finished message.
  ERR_SSL_DECRYPT_ERROR_ALERT = -153,

  // There are too many pending WebSocketJob instances, so the new job was not
  // pushed to the queue.
  ERR_WS_THROTTLE_QUEUE_TOO_LARGE = -154,

  // Error -155 was removed (TOO_MANY_SOCKET_STREAMS)

  // The SSL server certificate changed in a renegotiation.
  ERR_SSL_SERVER_CERT_CHANGED = -156,

  // Error -157 was removed (SSL_INAPPROPRIATE_FALLBACK).

  // Error -158 was removed (CT_NO_SCTS_VERIFIED_OK).

  // The SSL server sent us a fatal unrecognized_name alert.
  ERR_SSL_UNRECOGNIZED_NAME_ALERT = -159,

  // Failed to set the socket's receive buffer size as requested.
  ERR_SOCKET_SET_RECEIVE_BUFFER_SIZE_ERROR = -160,

  // Failed to set the socket's send buffer size as requested.
  ERR_SOCKET_SET_SEND_BUFFER_SIZE_ERROR = -161,

  // Failed to set the socket's receive buffer size as requested, despite success
  // return code from setsockopt.
  ERR_SOCKET_RECEIVE_BUFFER_SIZE_UNCHANGEABLE = -162,

  // Failed to set the socket's send buffer size as requested, despite success
  // return code from setsockopt.
  ERR_SOCKET_SEND_BUFFER_SIZE_UNCHANGEABLE = -163,

  // Failed to import a client certificate from the platform store into the SSL
  // library.
  ERR_SSL_CLIENT_AUTH_CERT_BAD_FORMAT = -164,

  // Error -165 was removed (SSL_FALLBACK_BEYOND_MINIMUM_VERSION).

  // Resolving a hostname to an IP address list included the IPv4 address
  // "127.0.53.53". This is a special IP address which ICANN has recommended to
  // indicate there was a name collision, and alert admins to a potential
  // problem.
  ERR_ICANN_NAME_COLLISION = -166,

  // The SSL server presented a certificate which could not be decoded. This is
  // not a certificate error code as no X509Certificate object is available. This
  // error is fatal.
  ERR_SSL_SERVER_CERT_BAD_FORMAT = -167,

  // Certificate Transparency: Received a signed tree head that failed to parse.
  ERR_CT_STH_PARSING_FAILED = -168,

  // Certificate Transparency: Received a signed tree head whose JSON parsing was
  // OK but was missing some of the fields.
  ERR_CT_STH_INCOMPLETE = -169,

  // The attempt to reuse a connection to send proxy auth credentials failed
  // before the AuthController was used to generate credentials. The caller should
  // reuse the controller with a new connection. This error is only used
  // internally by the network stack.
  ERR_UNABLE_TO_REUSE_CONNECTION_FOR_PROXY_AUTH = -170,

  // Certificate Transparency: Failed to parse the received consistency proof.
  ERR_CT_CONSISTENCY_PROOF_PARSING_FAILED = -171,

  // The SSL server required an unsupported cipher suite that has since been
  // removed. This error will temporarily be signaled on a fallback for one or two
  // releases immediately following a cipher suite's removal, after which the
  // fallback will be removed.
  ERR_SSL_OBSOLETE_CIPHER = -172,

  // When a WebSocket handshake is done successfully and the connection has been
  // upgraded, the URLRequest is cancelled with this error code.
  ERR_WS_UPGRADE = -173,

  // Socket ReadIfReady support is not implemented. This error should not be user
  // visible, because the normal Read() method is used as a fallback.
  ERR_READ_IF_READY_NOT_IMPLEMENTED = -174,

  // Error -175 was removed (SSL_VERSION_INTERFERENCE).

  // No socket buffer space is available.
  ERR_NO_BUFFER_SPACE = -176,

  // There were no common signature algorithms between our client certificate
  // private key and the server's preferences.
  ERR_SSL_CLIENT_AUTH_NO_COMMON_ALGORITHMS = -177,

  // TLS 1.3 early data was rejected by the server. This will be received before
  // any data is returned from the socket. The request should be retried with
  // early data disabled.
  ERR_EARLY_DATA_REJECTED = -178,

  // TLS 1.3 early data was offered, but the server responded with TLS 1.2 or
  // earlier. This is an internal error code to account for a
  // backwards-compatibility issue with early data and TLS 1.2. It will be
  // received before any data is returned from the socket. The request should be
  // retried with early data disabled.
  //
  // See https://tools.ietf.org/html/rfc8446#appendix-D.3 for details.
  ERR_WRONG_VERSION_ON_EARLY_DATA = -179,

  // TLS 1.3 was enabled, but a lower version was negotiated and the server
  // returned a value indicating it supported TLS 1.3. This is part of a security
  // check in TLS 1.3, but it may also indicate the user is behind a buggy
  // TLS-terminating proxy which implemented TLS 1.2 incorrectly. (See
  // https://crbug.com/boringssl/226.)
  ERR_TLS13_DOWNGRADE_DETECTED = -180,

  // The server's certificate has a keyUsage extension incompatible with the
  // negotiated TLS key exchange method.
  ERR_SSL_KEY_USAGE_INCOMPATIBLE = -181,

  // The ECHConfigList fetched over DNS cannot be parsed.
  ERR_INVALID_ECH_CONFIG_LIST = -182,

  // ECH was enabled, but the server was unable to decrypt the encrypted
  // ClientHello.
  ERR_ECH_NOT_NEGOTIATED = -183,

  // ECH was enabled, the server was unable to decrypt the encrypted ClientHello,
  // and additionally did not present a certificate valid for the public name.
  ERR_ECH_FALLBACK_CERTIFICATE_INVALID = -184,

  // Certificate error codes
  //
  // The values of certificate error codes must be consecutive.

  // The server responded with a certificate whose common name did not match
  // the host name.  This could mean:
  //
  // 1. An attacker has redirected our traffic to their server and is
  //    presenting a certificate for which they know the private key.
  //
  // 2. The server is misconfigured and responding with the wrong cert.
  //
  // 3. The user is on a wireless network and is being redirected to the
  //    network's login page.
  //
  // 4. The OS has used a DNS search suffix and the server doesn't have
  //    a certificate for the abbreviated name in the address bar.
  //
  ERR_CERT_COMMON_NAME_INVALID = -200,

  // The server responded with a certificate that, by our clock, appears to
  // either not yet be valid or to have expired.  This could mean:
  //
  // 1. An attacker is presenting an old certificate for which they have
  //    managed to obtain the private key.
  //
  // 2. The server is misconfigured and is not presenting a valid cert.
  //
  // 3. Our clock is wrong.
  //
  ERR_CERT_DATE_INVALID = -201,

  // The server responded with a certificate that is signed by an authority
  // we don't trust.  The could mean:
  //
  // 1. An attacker has substituted the real certificate for a cert that
  //    contains their public key and is signed by their cousin.
  //
  // 2. The server operator has a legitimate certificate from a CA we don't
  //    know about, but should trust.
  //
  // 3. The server is presenting a self-signed certificate, providing no
  //    defense against active attackers (but foiling passive attackers).
  //
  ERR_CERT_AUTHORITY_INVALID = -202,

  // The server responded with a certificate that contains errors.
  // This error is not recoverable.
  //
  // MSDN describes this error as follows:
  //   "The SSL certificate contains errors."
  // NOTE: It's unclear how this differs from ERR_CERT_INVALID. For consistency,
  // use that code instead of this one from now on.
  //
  ERR_CERT_CONTAINS_ERRORS = -203,

  // The certificate has no mechanism for determining if it is revoked.  In
  // effect, this certificate cannot be revoked.
  ERR_CERT_NO_REVOCATION_MECHANISM = -204,

  // Revocation information for the security certificate for this site is not
  // available.  This could mean:
  //
  // 1. An attacker has compromised the private key in the certificate and is
  //    blocking our attempt to find out that the cert was revoked.
  //
  // 2. The certificate is unrevoked, but the revocation server is busy or
  //    unavailable.
  //
  ERR_CERT_UNABLE_TO_CHECK_REVOCATION = -205,

  // The server responded with a certificate has been revoked.
  // We have the capability to ignore this error, but it is probably not the
  // thing to do.
  ERR_CERT_REVOKED = -206,

  // The server responded with a certificate that is invalid.
  // This error is not recoverable.
  //
  // MSDN describes this error as follows:
  //   "The SSL certificate is invalid."
  //
  ERR_CERT_INVALID = -207,

  // The server responded with a certificate that is signed using a weak
  // signature algorithm.
  ERR_CERT_WEAK_SIGNATURE_ALGORITHM = -208,

  // -209 is availible: was CERT_NOT_IN_DNS.

  // The host name specified in the certificate is not unique.
  ERR_CERT_NON_UNIQUE_NAME = -210,

  // The server responded with a certificate that contains a weak key (e.g.
  // a too-small RSA key).
  ERR_CERT_WEAK_KEY = -211,

  // The certificate claimed DNS names that are in violation of name constraints.
  ERR_CERT_NAME_CONSTRAINT_VIOLATION = -212,

  // The certificate's validity period is too long.
  ERR_CERT_VALIDITY_TOO_LONG = -213,

  // Certificate Transparency was required for this connection, but the server
  // did not provide CT information that complied with the policy.
  ERR_CERTIFICATE_TRANSPARENCY_REQUIRED = -214,

  // The certificate chained to a legacy Symantec root that is no longer trusted.
  // https://g.co/chrome/symantecpkicerts
  ERR_CERT_SYMANTEC_LEGACY = -215,

  // -216 was QUIC_CERT_ROOT_NOT_KNOWN which has been renumbered to not be in the
  // certificate error range.

  // The certificate is known to be used for interception by an entity other
  // the device owner.
  ERR_CERT_KNOWN_INTERCEPTION_BLOCKED = -217,

  // -218 was SSL_OBSOLETE_VERSION which is not longer used. TLS 1.0/1.1 instead
  // cause SSL_VERSION_OR_CIPHER_MISMATCH now.

  // Add new certificate error codes here.
  //
  // Update the value of CERT_END whenever you add a new certificate error
  // code.

  // The value immediately past the last certificate error code.
  ERR_CERT_END = -219
};

NetError SystemErrorToNetError(int32_t os_error);

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_BASE_NET_ERROR_H_
