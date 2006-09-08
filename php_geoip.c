/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2004 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: JoungKYun.Kim <http://www.oops.org>                          |
  +----------------------------------------------------------------------+

  $Id: php_geoip.c,v 1.4 2006-09-08 16:33:35 oops Exp $
*/

/*
 * PHP5 GeoIP module "geoip"
 */

#define EXTENSION_VERSION "1.1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <GeoIP.h>
#include <GeoIPCity.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_geoip.h"



ZEND_DECLARE_MODULE_GLOBALS(geoip)

static int le_geoip;


function_entry geoip_functions[] = {
   PHP_FE(geoip_open,                 NULL)   
   PHP_FE(geoip_close,                NULL)   
   PHP_FE(geoip_database_info,        NULL)   
   PHP_FE(geoip_db_avail,             NULL)   
   PHP_FE(geoip_country_code_by_name, NULL)
   PHP_FE(geoip_country_name_by_name, NULL)
   PHP_FE(geoip_id_by_name,           NULL)
   PHP_FE(geoip_record_by_name,       NULL)
   PHP_FE(geoip_org_by_name,          NULL)
   {NULL, NULL, NULL}   
};
/* }}} */

/* {{{ geoip_module_entry
 */
zend_module_entry geoip_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
   STANDARD_MODULE_HEADER,
#endif
   "geoip",
   geoip_functions,
   PHP_MINIT(geoip),
   PHP_MSHUTDOWN(geoip),
   PHP_RINIT(geoip),      
   PHP_RSHUTDOWN(geoip),   
   PHP_MINFO(geoip),
#if ZEND_MODULE_API_NO >= 20010901
   EXTENSION_VERSION, /* version number of the extension */
#endif
   STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GEOIP
ZEND_GET_MODULE(geoip)
#endif


#ifdef GEOIPDATADIR
#define GEOIPDATABASE GEOIPDATADIR "/GeoIP.dat"
#else
#define GEOIPDATABASE "GeoIP.dat"
#endif

/* }}} */

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
   STD_PHP_INI_ENTRY("geoip.database_standard", GEOIPDATABASE, PHP_INI_ALL, OnUpdateString, database_standard, zend_geoip_globals, geoip_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_geoip_init_globals
 */
static void php_geoip_init_globals(zend_geoip_globals *geoip_globals)
{
   geoip_globals->database_standard = NULL;
}
/* }}} */

/* {{{ _close_geoip_link (zend_rsrc_list_entry *rsrc TSRMLS_DC)
 */
static void _close_geoip_link (zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	GeoIP_API *ge = (GeoIP_API *) rsrc->ptr;
	GeoIP_delete (ge->gi);
	efree (ge);
	ge = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(geoip)
{
   ZEND_INIT_MODULE_GLOBALS(geoip, php_geoip_init_globals, NULL);
   REGISTER_INI_ENTRIES();

   le_geoip = zend_register_list_destructors_ex (_close_geoip_link, NULL, "GeoIP link", module_number);

   /* define geoip open method */
   REGISTER_LONG_CONSTANT ("GEOIP_STANDARD", GEOIP_STANDARD, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_MEMORY_CACHE", GEOIP_MEMORY_CACHE, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_CHECK_CACHE", GEOIP_CHECK_CACHE, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_INDEX_CACHE", GEOIP_INDEX_CACHE, CONST_PERSISTENT | CONST_CS);

   /* define geoip database type */
   REGISTER_LONG_CONSTANT ("GEOIP_COUNTRY_EDITION", GEOIP_COUNTRY_EDITION, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_REGION_EDITION_REV0", GEOIP_REGION_EDITION_REV0, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_CITY_EDITION_REV0", GEOIP_CITY_EDITION_REV0, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_ORG_EDITION", GEOIP_ORG_EDITION, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_ISP_EDITION", GEOIP_ISP_EDITION, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_CITY_EDITION_REV1", GEOIP_CITY_EDITION_REV1, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_REGION_EDITION_REV1", GEOIP_REGION_EDITION_REV1, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_PROXY_EDITION", GEOIP_PROXY_EDITION, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_ASNUM_EDITION", GEOIP_ASNUM_EDITION, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_NETSPEED_EDITION", GEOIP_NETSPEED_EDITION, CONST_PERSISTENT | CONST_CS);
   REGISTER_LONG_CONSTANT ("GEOIP_DOMAIN_EDITION", GEOIP_DOMAIN_EDITION, CONST_PERSISTENT | CONST_CS);

   return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(geoip)
{
   return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(geoip)
{
   return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(geoip)
{
   return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(geoip)
{
   php_info_print_table_start();
   php_info_print_table_header(2, "geoip support", "enabled");
   php_info_print_table_row(2, "geoip extension version", EXTENSION_VERSION);
   php_info_print_table_row(2, "geoip extension support", "COUNTRY,CITY,ISP,ORG");
   php_info_print_table_end();
   DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ geoip_open (database|database_type, flag)
 */
PHP_FUNCTION(geoip_open)
{
	zval ** database, ** _flag;
	char *dbname;
	int dbtype = 0;
	int dbl = 0;
	struct stat f;
	int flag;
	int r;
	int ge_argc;

	GeoIP_API * ge;

	ge_argc = ZEND_NUM_ARGS ();
	switch (ge_argc) {
		case 2:
			if ( zend_get_parameters_ex (2, &database, &_flag) == FAILURE )
				WRONG_PARAM_COUNT;

			if ( Z_TYPE_PP (database) == IS_STRING ) {
				convert_to_string_ex (database);
				dbname = Z_STRVAL_PP (database);

				if ( ! strlen (dbname) ) {
					php_error (E_WARNING, "length of %s is zero", database);
					RETURN_FALSE;
				}

				r = stat (dbname, &f);

				if ( r == -1 || f.st_size < 1 ) {
					php_error (E_WARNING, "%s not found or size is zoro", database);
					RETURN_FALSE;
				}

				dbl = f.st_size;
			} else {
				convert_to_long_ex (database);
				dbtype = Z_LVAL_PP (database);
				dbl = -1;
			}

			convert_to_long_ex (_flag);
			flag = Z_LVAL_PP (_flag);

			break;
		case 1:
			if ( zend_get_parameters_ex (1, &_flag) == FAILURE )
				WRONG_PARAM_COUNT;

			convert_to_long_ex (_flag);
			flag = Z_LVAL_PP (_flag);

			break;
		default:
			flag = GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE;
	}

	/*
	if ( ! dbl && GEOIP_G (database_standard) ) {
		dbname = GEOIP_G (database_standard);
		dbl = 1;
	}
	*/

	ge = (GeoIP_API *) emalloc (sizeof (GeoIP_API));

	ge->type = dbtype;

	if ( ! dbl ) {
		ge->gi = GeoIP_new (flag);
	} else {
		if ( dbl != -1 )
			ge->gi = GeoIP_open (dbname, flag);
		else
			ge->gi = GeoIP_open_type (dbtype, flag);
	}

	if ( ge->gi == NULL ) {
		RETURN_FALSE;
	}

	ZEND_REGISTER_RESOURCE (return_value, ge, le_geoip);
}
/* }}} */

/* {{{ geoip_close (geoip_stream)
 */
PHP_FUNCTION(geoip_close)
{
	zval **ge_link;
	GeoIP_API *ge;

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex (1, &ge_link) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, -1, "GeoIP link", le_geoip);
	zend_list_delete (Z_RESVAL_PP (ge_link));
}
/* }}} */

/* {{{ geoip_database_info
 */
PHP_FUNCTION(geoip_database_info)
{
	zval **ge_link;
	GeoIP_API *ge;
	char * db_info;

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex (1, &ge_link) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, -1, "GeoIP link", le_geoip);

	db_info = GeoIP_database_info (ge->gi);

	RETVAL_STRING (db_info, 1);
	free (db_info);
}
/* }}} */

/* {{{ geoip_db_avil
 */
PHP_FUNCTION(geoip_db_avail)
{
	zval **ge_type;
	int type = 0;
	int res  = 0;

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex (1, &ge_type) == FAILURE )
				WRONG_PARAM_COUNT;

			convert_to_long_ex (ge_type);
			type = Z_LVAL_PP (ge_type);

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	_GeoIP_setup_dbfilename();
	RETVAL_LONG(GeoIP_db_avail (type));

	for ( i = 1; i <= 11 ; i++ )
		free (GeoIPDBFileName[i]);
	free (GeoIPDBFileName);
}
/* }}} */

/* {{{ geoip_country_code_by_name
 */
PHP_FUNCTION(geoip_country_code_by_name)
{
	zval **ge_link, **host;
	GeoIP_API *ge;
	char * hostname = NULL;
	const char * country_code;
	int arglen;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex (2, &ge_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, -1, "GeoIP link", le_geoip);

	convert_to_string_ex (host);
	hostname = Z_STRVAL_PP (host);

	country_code = GeoIP_country_code_by_name (ge->gi, hostname);

	if ( country_code == NULL )
		RETURN_EMPTY_STRING ();

	RETURN_STRING ((char*) country_code, 1);
}
/* }}} */

/* {{{ geoip_country_name_by_name
 */
PHP_FUNCTION(geoip_country_name_by_name)
{
	zval **ge_link, **host;
	GeoIP_API *ge;
	char * hostname = NULL;
	const char * country_name;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex (2, &ge_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, -1, "GeoIP link", le_geoip);

	convert_to_string_ex (host);
	hostname = Z_STRVAL_PP (host);

	country_name = GeoIP_country_name_by_name (ge->gi, hostname);

   
	if ( country_name == NULL )
		RETURN_EMPTY_STRING ();

	RETURN_STRING ((char*) country_name, 1);
}
/* }}} */

/* {{{ gepip_id_by_name
 */
PHP_FUNCTION(geoip_id_by_name)
{
	zval **ge_link, **host;
	GeoIP_API *ge;
	char * hostname = NULL;
	int country_id;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex (2, &ge_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, -1, "GeoIP link", le_geoip);

	convert_to_string_ex (host);
	hostname = Z_STRVAL_PP (host);

	country_id = GeoIP_id_by_name (ge->gi, hostname);

	if ( array_init (return_value) == FAILURE ) {
		php_error (E_WARNING, "Failure array init");
		RETURN_FALSE;
	}

	add_assoc_long (return_value, "country_id", country_id);
	add_assoc_string (return_value, "code",
			GeoIP_country_code[country_id] ? GeoIP_country_code[country_id] : "--", 1);
	add_assoc_string (return_value, "name",
			GeoIP_country_name[country_id] ? GeoIP_country_name[country_id] : "N/A", 1);
}
/* }}} */

/* {{{ geoip_record_by_name
 */
PHP_FUNCTION(geoip_record_by_name)
{
	zval **ge_link, **host;
	GeoIP_API *ge;
	GeoIPRecord *gir;
	char * hostname = NULL;
	const char * country_name;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex (2, &ge_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, -1, "GeoIP link", le_geoip);

	convert_to_string_ex (host);
	hostname = Z_STRVAL_PP (host);

	gir = GeoIP_record_by_name (ge->gi, hostname);

	if ( gir == NULL )
		RETURN_EMPTY_STRING ();

	if ( array_init (return_value) == FAILURE ) {
		php_error (E_WARNING, "Failure array init");
		RETURN_FALSE;
	}

	add_assoc_string (return_value, "country_code", gir->country_code ? gir->country_code : "", 1);
	add_assoc_string (return_value, "region", gir->region ? gir->region : "", 1);
	add_assoc_string (return_value, "city", gir->city ? gir->city : "", 1);
	add_assoc_long (return_value, "postal_code", gir->postal_code ? gir->postal_code : 0);
	add_assoc_double (return_value, "latitude", gir->latitude ? gir->latitude : 0);
	add_assoc_double (return_value, "longitude", gir->longitude ? gir->longitude : 0);
	if ( ge->type == GEOIP_CITY_EDITION_REV1 ) {
		add_assoc_long (return_value, "dma_code", gir->dma_code ? gir->dma_code : 0);
		add_assoc_long (return_value, "area_code", gir->area_code ? gir->area_code : 0);
	}

	GeoIPRecord_delete (gir);
}
/* }}} */

/* {{{ geoip_org_by_name
 */
PHP_FUNCTION(geoip_org_by_name)
{
	zval **ge_link, **host;
	GeoIP_API *ge;
	char * hostname = NULL;
	const char * name;

	switch (ZEND_NUM_ARGS ()) {
		case 2:
			if ( zend_get_parameters_ex (2, &ge_link, &host) == FAILURE )
				WRONG_PARAM_COUNT;

			break;
		default:
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (ge, GeoIP_API *, ge_link, -1, "GeoIP link", le_geoip);

	convert_to_string_ex (host);
	hostname = Z_STRVAL_PP (host);

	name = GeoIP_org_by_name (ge->gi, hostname);

	if ( name == NULL )
		RETURN_EMPTY_STRING ();

	RETVAL_STRING ((char*) name, 1);
	free (name);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
