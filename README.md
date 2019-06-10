mod_geoip PHP extension
===
[![GPL2 license](https://img.shields.io/badge/license-GPLv2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)
![GitHub download](https://img.shields.io/github/downloads/OOPS-ORG-PHP/mod_geoip/total.svg)
[![GitHub last release](https://img.shields.io/github/release/OOPS-ORG-PHP/mod_geoip.svg)](https://github.com/OOPS-ORG-PHP/mod_geoip/releases)
[![GitHub closed issues](https://img.shields.io/github/issues-closed-raw/OOPS-ORG-PHP/mod_geoip.svg)](https://github.com/OOPS-ORG-PHP/mod_geoip/issues?q=is%3Aissue+is%3Aclosed)
[![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed-raw/OOPS-ORG-PHP/mod_geoip.svg)](https://github.com/OOPS-ORG-PHP/mod_geoip/pulls?q=is%3Apr+is%3Aclosed)

## DESCRIPTION

This extension is php extension of Maxmind's GeoIP library. But,
It is not same php geoip extension of [Maxmind](http://maxmind.com)

It has same functions, but has different code with [maxmind.com's](https://github.com/maxmind/geoip-api-php)

## DEPENDENCIES

This module requires these libraries:

 * PHP >= 7
  * If you want to PHP <= 5, use [v1 branch](https://github.com/OOPS-ORG-PHP/mod_geoip/tree/v1).
 * [GeoIP C API](https://github.com/maxmind/geoip-api-c)

## INSTALLATION

To install this extension type the following:

```bash
[root@host mod_krisp]$ phpize
[root@host mod_krisp]$ ./configure
[root@host mod_krisp]$ make && make install
```

## Usage

See also [doc directory](https://github.com/OOPS-ORG-PHP/mod_geoip/tree/master/vim/doc)

## COPYRIGHT AND LICENCE

Copyright (C) 2019 by JoungKyun.Kim &lt;https://joungkyun.github.com&gt;

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
