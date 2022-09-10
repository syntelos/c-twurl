
syntelos-twurl

  Yet another "twurl", this one in C.  A few vital twitter
  curl operations for the benefit of shell scripts that do
  twitter work.

  The JSON data received from the twitter API is "condensed"
  into an internal data table format.  The internal data
  table is displayed using "print" or filed using "write
  <file>".  The command line or console input shell performs
  multiple operations.

  The command line "app get <url> print" will show the
  internal data table for that operation.

  The command line "user get <url> valueof html" will
  extract a result from the operation.

Configuration

  The environment variable TWURL_STORE defines a directory
  containing the following files.

    api_key.txt
    api_key_secret.txt
    bearer_token.txt

  The 'api' files must be created and configured manually.

  The 'bearer' file may be subsequently created using 'twurl
  update'.

Build

  Run 'make'.  Uses 'clang'.

  Requires "libcurl".
    http://curl.se/

  Requires "libreadline" (Gnu Readline).
    apt-get install libreadline-dev

  Includes "json parser".
    https://github.com/json-parser/json-parser

Usage

  twurl -

    Enter console input shell.

  twurl user get <url>

    Employ bearer token to fetch URL to internal data table.

  twurl app get <url>

    Employ api/consumer secret to fetch URL to internal data
    table.

  twurl update

    Fetch new bearer token to TWURL_STORE
    'bearer_token.txt'.  Wipes the internal data table.

  twurl delete <index>

    Truncate element from internal data table.

  twurl print

    Write internal data table to console.

  twurl write <file>

    Write internal data table to file.


  These operations can be chained together on the command
  line, or entered serially on the console.
