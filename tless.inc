tless ()
{
    author 'Kevin Ernst'
    about "like 'less' for tab-delimited data files"
    param '$1 - the (tab-delimited) input file'
    example 'tless TAB_DELIMITED_FILE'
    example 'other-programs-stdout | tless'
    group 'util'

    # first, use AWK to replace all empty fields with '.'
    # then, use 'column' to make a table (-t), using ONLY tabs as delims (-s)
    # finally, pipe through 'less' without breaking long lines (-S)
    awk \
        -F$'\t' -vOFS='\t' -vNULL="$nullchar" \
        '{for(i=1; i<=NF; i++){if($i==""){$i="."}};print}' \
        "${1:-}" \
      | column -s $'\t' -t \
      | less -S
}

alias tl='tless'

# vim: ft=sh
