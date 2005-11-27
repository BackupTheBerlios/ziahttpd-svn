//
// modhelper.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 22:26:25 2005 
// Last update Fri Nov 25 13:37:42 2005 texane
//


#ifndef SERVER_MODHELPER_HH
# define SERVER_MODHELPER_HH


// Export a function with this macro
#if defined (_WIN32)
# define MOD_EXPORT(f)		extern "C" __declspec(dllexport) bool f
#else
# define MOD_EXPORT(f)		extern "C" bool f
#endif // _WIN32

// Hooked stage
# define HK_CREATE_CONNECTION	hk_create_connection__
# define HK_GET_RQST_METADATA	hk_get_rqst_metadata__
# define HK_GET_RQST_DATA	hk_get_rqst_data__
# define HK_PARSE_RQST_METADATA	hk_parse_rqst_metadata__
# define HK_ALTER_RQST_DATA	hk_alter_rqst_data__
# define HK_BUILD_RESP_METADATA	hk_build_resp_metadata__
# define HK_BUILD_RESP_DATA	hk_build_resp_data__
# define HK_ALTER_RESP_DATA	hk_alter_resp_data__
# define HK_ALTER_RESP_METADATA	hk_alter_resp_metadata__
# define HK_SEND_RESPONSE	hk_send_response__
# define HK_RELEASE_CONNECTION	hk_release_connection__

# define HK_STRFY( s )		#s
# define HK_SYM( sym )		HK_STRFY( sym )


#endif // ! SERVER_MODHELPER_HH
