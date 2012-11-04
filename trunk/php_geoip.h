/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2006 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: JoungKyun.Kim <http://www.oops.org>                          |
  +----------------------------------------------------------------------+

  $Id$
*/

#ifndef PHP_GEOIP_H
#define PHP_GEOIP_H

extern zend_module_entry geoip_module_entry;
#define phpext_geoip_ptr &geoip_module_entry

#ifdef PHP_WIN32
#define PHP_GEOIP_API __declspec(dllexport)
#else
#define PHP_GEOIP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <GeoIP.h>
#include <GeoIPCity.h>

PHP_MINIT_FUNCTION(geoip);
PHP_MSHUTDOWN_FUNCTION(geoip);
PHP_RINIT_FUNCTION(geoip);
PHP_RSHUTDOWN_FUNCTION(geoip);
PHP_MINFO_FUNCTION(geoip);


PHP_FUNCTION(geoip_open);
PHP_FUNCTION(geoip_close);
PHP_FUNCTION(geoip_database_info);
PHP_FUNCTION(geoip_db_avail);
PHP_FUNCTION(geoip_country_code_by_name);
PHP_FUNCTION(geoip_country_name_by_name);
PHP_FUNCTION(geoip_id_by_name);
PHP_FUNCTION(geoip_record_by_name);
PHP_FUNCTION(geoip_org_by_name);


ZEND_BEGIN_MODULE_GLOBALS(geoip)
	char* database_standard;
ZEND_END_MODULE_GLOBALS(geoip)

#ifdef ZTS
#define GEOIP_G(v) TSRMG(geoip_globals_id, zend_geoip_globals *, v)
#else
#define GEOIP_G(v) (geoip_globals.v)
#endif

typedef struct geo_resource {
	GeoIP *gi;
	int type;
	int rsrc;
} GeoIP_API;

#endif /* PHP_GEOIP_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

