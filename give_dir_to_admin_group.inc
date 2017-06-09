give_dir_to_admin_group ()
{
    author 'Kevin Ernst'
    about 'Gives (inherited) read/write/execute permissions to a directory'
    param '$1 - the directory on which to set permissions'
    param '$2 - (optional) group to grant access; default: oA-SA-<hostname>'
    param '-r - recursively set permissions for the named group'
    example 'give_dir_to_admin_group .'
    group 'sysadmin'

    # see which host we're on, and whether a sysadmin group exists
    prefix=oA-SA-
    hostname=$(hostname -s)
    admingroup=$prefix$hostname
    dir=
    recursive=false

    if [ $# -eq 0 -o $# -gt 3 ]; then
        echo "$(reference give_dir_to_admin_group)" >&2
        echo >&2
        echo "ERROR: wrong number of arguments (min: 1, max: 3)" >&2
        return 1
    fi

    while (( $# )); do
        if [ -d "$1" ]; then
            dir=$1
        elif getent group "$1" >/dev/null 2>&1; then
            admingroup=$1
        elif [[ $1 =~ ^--?r ]]; then
            recursive=true
        else
            echo "$(reference give_dir_to_admin_group)" >&2
            echo >&2
            echo "ERROR: unrecognized parameter '$1'" >&2
            return 1
        fi
        shift
    done

    # If user didn't supply a directory, bail out:
    if [ -z "$dir" ]; then
        echo "$(reference give_dir_to_admin_group)" >&2
        echo >&2
        echo 'ERROR: missing parameter $1 (dir to set permissions)' >&2
        return 1
    fi

    if [ "$recursive" = 'true' ]; then
        # I don't think quotes are required around the '{}' below, even if the
        # filename has spaces in it
        set -x
        find "$dir" -type f -exec sudo setfacl -m g:${admingroup}:rw {} +
        find "$dir" -type d -exec sudo setfacl -m g:${admingroup}:rwx {} +
        find "$dir" -type d -exec sudo setfacl -d -m g:${admingroup}:rwx {} +
        set +x
    else
        set -x
        sudo setfacl -d -m g:${admingroup}:rwx "$dir"
        sudo setfacl -m g:${admingroup}:rwx "$dir"
        set +x
    fi
}

# vim:ft=sh
