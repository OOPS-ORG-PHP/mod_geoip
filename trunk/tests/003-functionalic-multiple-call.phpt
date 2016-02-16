--TEST--
Check for geoip functionalic multiple test
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
$searches = array ('oops.org', 'kornet.net', 'dacom.net');

try {
	/* open conutry database */
	foreach ( $searches as $v ) {
		$g = GeoIP_open (GEOIP_MEMORY_CACHE|GEOIP_CHECK_CACHE);
		if ( ! is_resource ($g) )
			exit;

			$r = geoip_id_by_name ($g, $v);
			print_r ($r);

		/*
		 * close geoip database
		 */
		GeoIP_close ($g);
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
    [country_id] => 119
    [code] => KR
    [name] => Korea, Republic of
)
Array
(
    [country_id] => 119
    [code] => KR
    [name] => Korea, Republic of
)
Array
(
    [country_id] => 119
    [code] => KR
    [name] => Korea, Republic of
)
