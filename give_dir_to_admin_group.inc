give_dir_to_admin_group ()
{
    author 'Kevin Ernst'
    about 'Gives (inherited) read/write/execute permissions to a directory'
    param '$1 - the directory on which to set permissions'
    param '$2 - (optional) the group for which to grant access'
    param '-r - recursively set permissions for the named group'
    example 'give_dir_to_admin_group .'
    group 'sysadmin'

    # see which host we're on, and whether a sysadmin group exists
    prefix=oA-SA-
    hostname=$(hostname -s)
    admingroup=$prefix$hostname
    dir=
    recursive=false

    if [ $# -eq 0 -o $# -gt 2 ]; then
        echo $(reference give_dir_to_admin_group) >&2
        echo >&2
        echo "ERROR: wrong number of arguments (min: 1, max: 2)" >&2
        exit 1
    fi

    while (( $# )); do
        if [ -d "$1" ]; then
            dir=$1
        elif getent group "$1" >/dev/null 2>&1; then
            admingroup=$1
        elif [[ "$1" =~ ^--?r ]]; then
            recursive=true
        else
            echo $(reference give_dir_to_admin_group) >&2
            echo >&2
            echo "ERROR: unrecognized parameter '$1'" >&2
            exit 1
        fi
        shift
    done

    # If user didn't supply a directory, bail out:
    if [ -z "$dir" ]; then
        echo $(reference give_dir_to_admin_group) >&2
        echo >&2
        echo 'ERROR: missing parameter $1 (dir to set permissions)' >&2
        exit 1
    fi

    sudo setfacl -d -m g:o${admingroup}:rwx "$1"
    sudo setfacl -m g:o${admingroup}:rwx "$1"
    
    #if [ 
}

# vim:ft=sh
