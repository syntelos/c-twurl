
Description

  Perform twitter curl via "libcurl".

  Consolidating twitter curl to convenient code and
  configuration forms and formats is hindered by translating
  HTTP headers across shell functions and commands.  This C
  programming language program solves this problem.  Twitter
  curl operations are condensed and configuration is
  consolidated for the benefit of shell scripts that do
  twitter work.

Configuration

  The environment variable TWURL_STORE defines a directory
  containing the following files.

    api_key.txt
    api_key_secret.txt
    bearer_token.txt

Build

  Run 'make'.  Uses 'clang'.

  Requires "libcurl".
    http://curl.se/

  Requires "libreadline" (Gnu Readline).
    apt-get install libreadline-dev

  Includes "json parser".
    https://github.com/json-parser/json-parser

Usage

  twurl user get <url>

    Employ bearer token to fetch URL to internal data table.

  twurl app get <url>

    Employ api/consumer secret to fetch URL to internal data
    table.

  twurl update

    Fetch new bearer token to TWURL_STORE
    'bearer_token.txt'.

  twurl delete <index>

    Truncate element from internal data table.

  twurl print

    Write internal data table to console.

  twurl write <file>

    Write internal data table to file.

