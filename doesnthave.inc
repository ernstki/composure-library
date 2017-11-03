doesnthave ()
{
    author 'Kevin Ernst'
    about 'Find directories /not/ containing a named (non-empty) file'
    param '$1    - filename to search for'
    param '$2 $* - directories to search within (default: c.w.d. and subdirs)'
    example 'doesnthave snp_coords.bed'
    example 'doesnthave snpcoords.bed bf504a8af2 ba7a23c3bd'
    group 'lsf'

    # set -x
    # Shift off the first argument; that's the filename
    local file=$1
    local dirs=()
    shift

    test -z "$file" && \
        echo -e "ACK! You must specify at *least* a filename to search for.\n" >&2 && \
        return 1

    # Trim leading './'
    if (( $# )); then
        for dir in "$@"; do
            if [[ ! -d "$dir" ]]; then
                echo "WARN: '$dir' not a directory" >&2
                continue
            fi
            # echo "dir='$dir'"
            dirs=( "${dirs[@]}" "${dir#./}" )
        done
    else
        # Everything in the current directory.
        # Ref: https://stackoverflow.com/a/8677566
        while IFS= read -r -d '' dir; do
            # echo "dir='$dir'"
            dirs=( "${dirs[@]}" "$dir" )
        done < <(find * -maxdepth 0 -type d -print0)

        # This /almost/ worked, but doesn't preserve embedded spaces in dir names
        #dirs=( $(find * -maxdepth 0 -type d) )
    fi

    test -z "${dirs[*]}" && \
        echo -e "ACK! Invalid/empty list of directories.\n" >&2 && \
        return 1

    # Do it
    parallel -v cd {} \; test -s "$file" '||' echo {} ::: "${dirs[@]}"
    # set +x
}

# vim: ft=sh
