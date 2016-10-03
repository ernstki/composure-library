ega-decrypt ()
{
    author 'Kevin Ernst'
    about 'Decrypt an AES-encrypted file from the EGA dataset(s)'
    param '$1 - filename with .aes extension to decrypt'
    param '$2 - (optional) secrets file'
    example 'ega-decrypt <dir>/datafile.txt.gz.aes'
    group 'ega'
    openssl enc -aes-256-cbc -d -pass file:${2:-secret.txt} \
            -in "$1" -out "${1%%.aes}"
}
