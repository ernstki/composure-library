gpg_fetch_key_from_asc ()
{
    author 'Kevin Ernst'
    about 'Retrieves a GPG key from a .asc file in the c.w.d.'
    param ''
    example '$ gpg_key_from_asc'
    example '$ gpg_key_from_asc curl-7.47.1.tar.gz.asc'
    group 'gpg'
    gpg_fetch_key $(gpg_key_from_asc)
}
