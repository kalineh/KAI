

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_NETWORK_SOCKET_H
#	define KAI_NETWORK_SOCKET_H

#	define KAI_NETWORK_NAMESPACE_NAME Network

#	define KAI_NETWORK_BEGIN \
		KAI_BEGIN \
		namespace KAI_NETWORK_NAMESPACE_NAME {

#	define KAI_NETWORK_END KAI_END \
		}

#	define KAI_PLATFORM_NETWORK_BEGIN \
		KAI_PLATFORM_BEGIN \
		namespace Network {

#	define KAI_PLATFORM_NETWORK_END \
		} \
		KAI_PLATFORM_END \

#endif // KAI_NETWORK_SOCKET_H

//EOF
