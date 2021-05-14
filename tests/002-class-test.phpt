--TEST--
Check for geoip classfy test
--SKIPIF--
<?php
if ( ! extension_loaded ('geoip') ) {
	print 'skip';
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php
$searches = array ('cnn.com', 'kornet.net', 'dacom.net');

/*
 * GeoIP::__construct (database, flag)
 * GeoIP::__construct (database_code, flag)
 * GeoIP::__construct (flag)
 * GeoIP::__construct
 * 
 * open the geoip database
 *
 * argument info:
 *      database => GeoIP datafile path or database code
 *           database code :
 *                  GEOIP_COUNTRY_EDITION    -> default
 *                  GEOIP_ISP_EDITION        -> isp database (if you have)
 *                  GEOIP_CITY_EDITION_REV0  -> city database (if you have)
 *                  GEOIP_CITY_EDITION_REV1  -> city database (if you have)
 *                  GEOIP_ORG_EDITION        -> org database (if you have)
 *
 *      flag     => GEOIP_STANDARD
 *                  GEOIP_MEMORY_CACHE
 *                  GEOIP_CHECK_CACHE
 *                  GEOIP_INDEX_CACHE
 *
 * if flag is not specifyed, default "GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE"
 *
 * if failed, return FALSE
 *
 */

try {
	/* open conutry database */
	$g = new GeoIP (GEOIP_MEMORY_CACHE|GEOIP_CHECK_CACHE);
	/* open city database */
	#if ( GeoIP_db_avail (GEOIP_CITY_EDITION_REV1) )
	#	$gc = new GeoIP (GEOIP_CITY_EDITION_REV1, GEOIP_INDEX_CACHE|GEOIP_CHECK_CACHE);
	/* open isp database */
	if ( GeoIP_db_avail (GEOIP_ISP_EDITION) )
		$gi = new GeoIP (GEOIP_ISP_EDITION, GEOIP_INDEX_CACHE|GEOIP_CHECK_CACHE);

	#echo "TYPE: " . $g->database_info () ."\n";

	foreach ( $searches as $v ) {
		/*
		 * search geoip country database
		 *
		 * array GeoIP::id_by_name (geoip handle, host);
		 * return:
		 *      array (
		 *              country_id,
		 *              code,        // code of nation
		 *              name         // name of nation
		 *            );
		 *
		 * string GeoIP::country_code_by_name (host)
		 * string GeoIP::country_code_by_name (host)
		 *
		 */
		$r = $g->id_by_name ($v);
		print_r ($r);

		/* print city information
		 *
		 * array GeoIP::record_by_name (host)
		 * return:
		 *      array (
		 *              country_code,  // country 2 digit code
		 *              region,        // states or do (korean)
		 *              city,          // name of city
		 *              postal_code,   // postal code or post code
		 *              latitude,
		 *              longitude,
		 *              if ( DB is GEOIP_CITY_EDITION_REV1 ) {
		 *                  dma_code,
		 *                  area_code,
		 *              }
		 *            );
		 */
		#if ( GeoIP_db_avail (GEOIP_CITY_EDITION_REV1) ) {
		#	$rc = $gc->record_by_name ($v);
		#	print_r ($rc);
		#}

		/* print isp information
		 *
		 * string GeoIP::org_by_name (host)
		 */
		if ( GeoIP_db_avail (GEOIP_ISP_EDITION) ) {
			$ri = $gi->org_by_name ($v);
			echo "ISP NAME: $ri\n";
		}

		#echo "### " . $g->country_code_by_name ($v) . "\n";
		#echo "### " . $g->country_name_by_name ($v) . "\n";
	}
} catch ( GeoIPException $e ) {
	fprintf (STDERR, "%s\n", $e->getMessage ());
	$err = preg_split ('/\r?\n/', $e->getTraceAsString ());
	print_r ($err);
}
?>
--EXPECT--
Array
(
    [country_id] => 225
    [code] => US
    [name] => United States
)
ISP NAME: Fastly
Array
(
    [country_id] => 119
    [code] => KR
    [name] => Korea, Republic of
)
ISP NAME: Korea Telecom
Array
(
    [country_id] => 119
    [code] => KR
    [name] => Korea, Republic of
)
ISP NAME: DACOM Corp.
