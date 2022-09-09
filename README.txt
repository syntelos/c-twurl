
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

  Requires "libcurl".  See http://curl.se/ for more info.

Usage

  twurl user get <url>

    Employ bearer token to fetch URL to standard console
    output.

  twurl app get <url>

    Employ api/consumer secret and bearer token to fetch
    URL to standard console output.

  twurl update

    Fetch new bearer token to TWURL_STORE
    'bearer_token.txt'.

