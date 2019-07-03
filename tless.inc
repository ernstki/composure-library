tless ()
{
    author 'Kevin Ernst'
    about 'like "less" for tab-delimited data'
    param '$1 - filename of the tab-delimited file to display'
    example '$ tless input.bed'
    example '$ cut -f1,4 input.bed | tless'
    group 'util'

    local nullchar='.'
    local usage="
usage: tless TAB_DELIMITED_FILE
       other-programs-stdout | tless
"      
    # if fh 0 (stdin) is connected to a terminal, and # args <> 1
    if [[ -t 0 && $# -ne 1 ]]; then
        echo "$usage" >&2
        return 1
    fi

    # if stdin is the user's terminal, warn if input file has CR chars; $'\t'
    # is the POSIX trick for passing non-printables as literal arguments
    if [[ -t 0 ]]; then 
        if grep -q $'\r' "$1"; then
            cat >&2 <<ERRMSG

  *** DANGER, WILL ROBINSON!!! ***"

  DOS/Windows CR ('\\r') characters were found in the input file.
  Although this script will (mostly) tolerate that, most other Unix
  utilities ('cut', 'sed', etc.) might behave weirdly.

  It might be best if you were to fix that now with 'dos2unix' or
  "tr -d '\\r' <dosends.txt >unixends.txt" to avoid future problems
  and frustrations.

ERRMSG
            read -rp '  Press ENTER to continue or Ctrl+C to quit... '
        fi
    fi

    # first, use AWK to replace all empty fields with '.'
    # then, use 'column' to make a table (-t), using ONLY tabs as delims (-s)
    # finally, pipe through 'less' without breaking long lines (-S)
    awk \
        -F'\t' -v OFS='\t' -v NULL="$nullchar" \
        '{for(i=1; i<=NF; i++){if($i==""){$i=NULL}};print}' \
        "${1:-}" \
      | column -s $'\t' -t \
      | less -S
}

# vim: ft=sh
