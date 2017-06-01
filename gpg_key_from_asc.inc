gpg_key_from_asc ()
{
    author 'Kevin Ernst'
    about 'Extracts the GPG key ID from a .asc file using `gpg --verify`'
    param '1: (optional) the .asc GPG key file to inspect'
    example ''
    group 'gpg'

    local keyfile="$1"
    if [ -z "$1" ]; then
        keyfile=*.asc
    fi
        
    gpg --verify $keyfile 2>&1 | sed -n 's/.*ID \([[:xdigit:]]\{8\}\)/\1/p'
}
