doesnthave ()
{
    author 'Kevin Ernst'
    about 'Report directories /not/ containing a named (non-empty) file'
    param '$file           - filename to search for'
    param "--empty-ok      - (optional) don't report if \$file exists but is empty"
    param '--has-empty     - (optional) report only if $file exists and is empty'
    param '--has-non-empty - (optional) report only if $file exists and is NON-empty'
    param '<dir> ...       - (optional) search within dirs (default: c.w.d. and subdirs)'
    example '$ doesnthave --has-empty allresults.bed'
    example '$ doesnthave --empty-ok snp_coords.bed'
    example '$ doesnthave snpcoords.bed bf504a8af2 ba7a23c3bd'
    group 'lsf'

    # set -x
    local DEBUG=${DEBUG:+-v}
    # skip over (don't print) if this directory passes this test
    local SKIP_IF=
    # by default, consider empty files as "missing"
    local EMPTY_OK=
    # report *only* for existing empty files
    local REPORT_ONLY_EMPTY=
    # report *only* for existing non-empty files
    local REPORT_ONLY_NONEMPTY=
    local numflags=0
    local file=
    local dirs=()

    while (( $# )); do
        if [[ $1 =~ ^--?(allow-empty|empty-ok) ]]; then
            EMPTY_OK=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--?(only-|is-|has-)empty ]]; then
            REPORT_ONLY_EMPTY=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--?(only-|is-|has-)non-?empty ]]; then
            REPORT_ONLY_NONEMPTY=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--? ]]; then
                echo "ACK! Unrecognized option '$1'." >&2 && \
                    return 1
        elif [[ -d "$1" ]]; then
            test -n "$DEBUG" && echo "dir='$1'"
            # Trim leading './'
            dirs=( "${dirs[@]}" "${1#./}" )
        else
            # consider this a filename
            if [[ $file ]]; then
                echo "ACK! Multiple filenames not (yet) supported." >&2 && \
                    return 1
            fi
            file=$1
        fi
        shift
    done

    test -z "$file" && \
        echo -e "ACK! You must at *least* specify a filename to search for.\n" >&2 && \
        return 1

    # error out on mutually-exclusive options
    test $numflags -gt 1 && \
        echo -e "ACK! Only one '--emtpy-ok' or '--has-[non-]emtpy' option" \
                "is allowed.\n" >&2 && return 1

    # If *no* directories given on command line; add everything in the c.w.d.
    if [[ -z "${dirs[*]}" ]]; then
        # Ref: https://stackoverflow.com/a/8677566
        while IFS= read -r -d '' dir; do
            test -n "$DEBUG" && echo "dir='$dir'"
            dirs=( "${dirs[@]}" "$dir" )
        done < <(find * -maxdepth 0 -type d -print0)

        # This /almost/ worked, but doesn't preserve embedded spaces in dir names
        #dirs=( $(find * -maxdepth 0 -type d) )
    fi

    # Still nothing (no subdirectories)? Bail out.
    test -z "${dirs[*]}" && \
        echo -e "ACK! Invalid/empty list of directories.\n" >&2 && \
        return 1

    # The default behavior is "skip this directory if the file exists and it's
    # non-empty"
    SKIP_IF="test -s '$file'"

    if [[ $REPORT_ONLY_EMPTY ]]; then
        # skip if the file isn't there, or if it's non-zero size (only print if
        # the file exists *and* is zero size)
        SKIP_IF="test ! -f '$file' -o -s '$file'"
    elif [[ $REPORT_ONLY_NONEMPTY ]]; then
        # reverse the logic of the above (report only directories where the
        # file exists *and* has non-zero size)
        SKIP_IF="test ! -f '$file' -o ! -s '$file'"
    elif [[ $EMPTY_OK ]]; then
        # skip this directory if the file even exists (fine if it's empty)
        SKIP_IF="test -f '$file'"
    fi

    # Do it
    parallel $DEBUG cd {} \; $SKIP_IF '||' echo {} ::: "${dirs[@]}"
    # set +x
}

# vim: ft=sh
